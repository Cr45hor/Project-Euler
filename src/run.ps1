param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$Source,

    # Catches -Clean
    [switch]$Clean,

    # Collects any remaining arguments to pass directly to the C++ executable
    [Parameter(ValueFromRemainingArguments = $true)]
    [string[]]$ProgramArgs
)

$VersionFlag = "-std=c++17"
$WarningFlag = "-Wall", "-Wextra", "-Wshadow"
$OptimizationFlag = "-O3", "-march=native", "-flto"
$LinkerFlag = "-lgmpxx", "-lgmp", "-fopenmp"
$StackFlag = "-Wl,--stack,67108864"

$Exe = [System.IO.Path]::ChangeExtension($Source, ".exe")

$compileArgs = @($VersionFlag) + $WarningFlag + @($OptimizationFlag) + @($Source, "-o", $Exe) + $LinkerFlag + @($StackFlag)

Write-Output "$([char]27)[36mCompiling $Source...$([char]27)[0m"
& g++ @compileArgs

if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

$ExePath = if ([System.IO.Path]::IsPathRooted($Exe)) { $Exe } else { Join-Path (Get-Location) $Exe }

try {
    $sw = [System.Diagnostics.Stopwatch]::StartNew()

    # Splatting @ProgramArgs cleanly passes array elements as separate arguments
    if ($ProgramArgs.Count -gt 0) {
        & $ExePath @ProgramArgs
    } else {
        & $ExePath
    }

    $exitCode = $LASTEXITCODE
    $sw.Stop()

    if ($exitCode -ne 0) { exit $exitCode }

    Write-Host ("Execution time: {0:N6} seconds" -f $sw.Elapsed.TotalSeconds) -ForegroundColor Green
}
finally {
    if ($Clean -and (Test-Path $ExePath)) {
        Remove-Item $ExePath -Force
        Write-Host "Cleaned: $ExePath" -ForegroundColor DarkGray
    }
}