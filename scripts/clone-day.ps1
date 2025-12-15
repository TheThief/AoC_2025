param(
    [int]$SourceDay = 1,
    [int]$TargetDay = (Get-Date).Day,
    [string]$RepoRoot = (Get-Location).Path
)

function Replace-InFile {
    param($Path, $Replacements)
    $text = Get-Content -Raw -LiteralPath $Path -ErrorAction Stop

    # Process longest Old values first to avoid partial-overlap collisions
    $pairs = $Replacements | Sort-Object -Property @{ Expression = { $_.Old.Length } } -Descending
    foreach ($kv in $pairs) {
        $escapedKey = [regex]::Escape($kv.Old)
        # Case-sensitive replacement only
        $text = [regex]::Replace($text, $escapedKey, [System.Text.RegularExpressions.MatchEvaluator]{ param($m) $kv.New }, [System.Text.RegularExpressions.RegexOptions]::None)
    }

    Set-Content -LiteralPath $Path -Value $text -Encoding UTF8
}

$srcName = "AoC_2025_Day$SourceDay"
$dstName = "AoC_2025_Day$TargetDay"
$srcTestName = "${srcName}_Test"
$dstTestName = "${dstName}_Test"

$srcDir = Join-Path $RepoRoot $srcName
$dstDir = Join-Path $RepoRoot $dstName
$srcTestDir = Join-Path $RepoRoot $srcTestName
$dstTestDir = Join-Path $RepoRoot $dstTestName

if (-not (Test-Path $srcDir)) {
    Write-Error "Source directory '$srcDir' not found."
    exit 1
}
if (Test-Path $dstDir) {
    Write-Error "Target directory '$dstDir' already exists. Remove it or choose a different target."
    exit 1
}

Write-Output "Creating '$dstDir' and copying top-level files from '$srcDir'..."
# Create destination directory and copy only files directly in the source directory (non-recursive)
New-Item -ItemType Directory -Path $dstDir -Force | Out-Null
Get-ChildItem -LiteralPath $srcDir -File -Force | ForEach-Object {
    Copy-Item -LiteralPath $_.FullName -Destination $dstDir -Force
}

$copiedTest = $false
if (Test-Path $srcTestDir) {
    if (Test-Path $dstTestDir) {
        Write-Error "Target test directory '$dstTestDir' already exists. Remove it or choose a different target."
        exit 1
    }
    Write-Output "Creating '$dstTestDir' and copying top-level files from '$srcTestDir'..."
    New-Item -ItemType Directory -Path $dstTestDir -Force | Out-Null
    Get-ChildItem -LiteralPath $srcTestDir -File -Force | ForEach-Object {
        Copy-Item -LiteralPath $_.FullName -Destination $dstTestDir -Force
    }
    $copiedTest = $true
} else {
    Write-Output "No test directory found at '$srcTestDir'. Skipping test project copy."
}

# Prepare replacement list (simple list of Old/New entries)
$replacements = @(
    @{ Old = "AoC_2025_Day$SourceDay"; New = "AoC_2025_Day$TargetDay" },
    @{ Old = "${srcName}_Test";           New = "${dstName}_Test" },
    @{ Old = "aoc2025::day$SourceDay";    New = "aoc2025::day$TargetDay" },
    @{ Old = "AoC_2025_Day$SourceDay/AoC_2025_Day$SourceDay.h"; New = "AoC_2025_Day$TargetDay/AoC_2025_Day$TargetDay.h" },
    @{ Old = "AoC_2025_Day$SourceDay\\AoC_2025_Day$SourceDay.h"; New = "AoC_2025_Day$TargetDay\\AoC_2025_Day$TargetDay.h" },
    @{ Old = "day$SourceDay";             New = "day$TargetDay" },
    @{ Old = "Day$SourceDay";             New = "Day$TargetDay" }
)

# Update contents of copied project(s) — only files directly in the destination directories (non-recursive)
Get-ChildItem -LiteralPath $dstDir -File -Force | ForEach-Object {
    Replace-InFile -Path $_.FullName -Replacements $replacements
}
if ($copiedTest) {
    Get-ChildItem -LiteralPath $dstTestDir -File -Force | ForEach-Object {
        Replace-InFile -Path $_.FullName -Replacements $replacements
    }
}

# Rename destination files that include the source day number (non-recursive)
$files = @(Get-ChildItem -LiteralPath $dstDir -File -Force)
if ($copiedTest) { $files += @(Get-ChildItem -LiteralPath $dstTestDir -File -Force) }
$files | Sort-Object -Property FullName -Descending | ForEach-Object {
    if ($_.Name -match [regex]::Escape("$SourceDay")) {
        $newName = $_.Name -replace [regex]::Escape("$SourceDay"), "$TargetDay"
        Rename-Item -LiteralPath $_.FullName -NewName $newName
    }
}

# Rename .vcxproj files if they include the day — only in the top-level of the destination project directories
foreach ($proj in (Get-ChildItem -LiteralPath $dstDir -Filter "*.vcxproj" -File -ErrorAction SilentlyContinue)) {
    if ($proj.Name -match [regex]::Escape("$SourceDay")) {
        $newName = $proj.Name -replace [regex]::Escape("$SourceDay"), "$TargetDay"
        Rename-Item -LiteralPath $proj.FullName -NewName $newName
    }
}
if ($copiedTest) {
    foreach ($proj in (Get-ChildItem -LiteralPath $dstTestDir -Filter "*.vcxproj" -File -ErrorAction SilentlyContinue)) {
        if ($proj.Name -match [regex]::Escape("$SourceDay")) {
            $newName = $proj.Name -replace [regex]::Escape("$SourceDay"), "$TargetDay"
            Rename-Item -LiteralPath $proj.FullName -NewName $newName
        }
    }
}

# Prefer .slnx (XML) modification; no classic .sln fallback
$slnx = Get-ChildItem -LiteralPath $RepoRoot -Filter *.slnx -Recurse -File -ErrorAction SilentlyContinue | Select-Object -First 1
if ($slnx) {
    Write-Output "Found .slnx: $($slnx.FullName) -- updating XML Project entries at document root."
    try {
        [xml]$xml = Get-Content -LiteralPath $slnx.FullName -ErrorAction Stop

        # operate directly on the document element (root) — add Project nodes as direct children
        $root = $xml.DocumentElement

        # Build deterministic project list using destination names (no Get-ChildItem)
        $projFiles = @()
        $projFiles += "$dstName/$dstName.vcxproj"
        if ($copiedTest) { $projFiles += "$dstTestName/$dstTestName.vcxproj" }

        foreach ($p in $projFiles) {
            # p is a string like "AoC_2025_DayN/AoC_2025_DayN.vcxproj"
            # avoid duplicates by scanning root child nodes for a Project with matching Path attribute
            $exists = $false
            foreach ($child in $root.ChildNodes) {
                if ($child.LocalName -eq "Project" -and $child.Attributes -and $child.Attributes["Path"] -and ($child.Attributes["Path"].Value -ieq $p)) {
                    $exists = $true
                    break
                }
            }
            if (-not $exists) {
                $projElem = $xml.CreateElement("Project")
                $attr = $xml.CreateAttribute("Path")
                $attr.Value = $p
                $projElem.Attributes.Append($attr) | Out-Null
                $root.AppendChild($projElem) | Out-Null
                Write-Output "Added slnx Project entry: $p"
            } else {
                Write-Output "Entry already present in slnx: $p"
            }
        }

        # save updated .slnx (preserve UTF8)
        $xml.Save($slnx.FullName)
        Write-Output "Updated .slnx saved: $($slnx.FullName)"
    } catch {
        Write-Warning "Failed to update .slnx XML: $_. Projects were copied but not added to any solution."
    }
} else {
    Write-Output "No .slnx found under '$RepoRoot'. Projects were copied but not added to a solution."
}

Write-Output "Clone complete. Next steps:"
Write-Output "1) Open solution in Visual Studio and verify projects are present. If needed, refresh via __Solution Explorer__ and save the solution."
Write-Output "2) Build and fix any project references or includes as needed."