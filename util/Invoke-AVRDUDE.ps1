<#
.SYNOPSIS

Waits for a new serial port to appear, then invokes AVRDUDE using that port.

.DESCRIPTION

The Invoke-AVRDUDE.ps1 script waits for a new serial port to appear,
assuming that the newly added port is presented by the Caterina bootloader
running on an AVR-based USB device.  After detecting a new serial port the
script invokes the AVRDUDE tool, passing the "-P <port>" option and any
other options specified in the -avrdudeOptions parameter.

#>

param(
	[string]
	# Specifies the AVRDUDE executable file path.  Defaults to "$PSScriptRoot\wsl_downloaded\avrdude.exe", where $PSScriptRoot is the directory where the script file is located.
	$avrdudePath,

	[string]
	[Parameter(Mandatory = $true, Position = 0)]
	# Specifies the AVRDUDE command line options.  The "-P <port>" option will be automatically added before these options.
	$avrdudeOptions
)

# Using $PSScriptRoot in the default value of a parameter does not work.
if (!$avrdudePath) {
	$avrdudePath = "$PSScriptRoot\wsl_downloaded\avrdude.exe"
}

$portList = [System.IO.Ports.SerialPort]::GetPortNames()
$portHash = @{}
ForEach ($name in $portList) {
	$portHash.Add($name, $TRUE)
}

Write-Host -NoNewline "Detecting USB port, reset your controller now."
$newPort = $NULL
:waitForPort while ($TRUE) {
	Start-Sleep -Milliseconds 500
	$portList = [System.IO.Ports.SerialPort]::GetPortNames()
	ForEach ($name in $portList) {
		if (! $portHash.ContainsKey($name)) {
			$newPort = $name
			break waitForPort
		}
	}
	Write-Host -NoNewline "."
}

Write-Host ""
Write-Host "Device $newPort has appeared; assuming it is the controller."

Start-Sleep -Seconds 1

$p = Start-Process -Wait -NoNewWindow -FilePath $avrdudePath -ArgumentList "-P $newPort $avrdudeOptions" -PassThru
Exit $p.ExitCode