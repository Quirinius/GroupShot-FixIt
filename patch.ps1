# File and patch details
$filename = "MSRGS.exe"
$expectedFilesize = 1355776

# Define a patch: offset and byte(s) to write
$patches = @(
    @{ Offset = 0x00001D97; Data = [byte[]](0xEB) }
)

Write-Output "Patch is for MS Research GroupShot 1.03"
Write-Output ""

# Check file existence
if (-Not (Test-Path $filename)) {
    Write-Error "Unable to open '$filename' for writing!"
    exit 1
}

# Get actual file size
$actualSize = (Get-Item $filename).Length
if ($actualSize -ne $expectedFilesize) {
    Write-Error "Wrong file size. Expected $expectedFilesize bytes, but got $actualSize bytes."
    exit 1
}

# Open file for binary write
$fs = [System.IO.File]::Open($filename, 'Open', 'ReadWrite')
$bw = New-Object System.IO.BinaryWriter($fs)

# Apply each patch
for ($i = 0; $i -lt $patches.Count; $i++) {
    $patch = $patches[$i]
    Write-Output "Patching...step $i..."

    try {
        $fs.Seek($patch.Offset, [System.IO.SeekOrigin]::Begin) | Out-Null
        $bw.Write($patch.Data)

        if ($patch.Data.Length -eq 1) {
            Write-Output "ok!"
        } else {
            Write-Output "ok! ($($patch.Data.Length) bytes written)"
        }
    }
    catch {
        Write-Output "error! (file may be corrupt)"
    }
}

# Clean up
$bw.Close()
$fs.Close()

exit 0
