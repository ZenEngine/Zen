$scriptsDir = split-path -parent $script:MyInvocation.MyCommand.Definition
$rootDir = (get-item $scriptsDir).parent.FullName

$cmakeSourceDir = $rootDir
$cmakeBuildDir = Join-Path $rootDir "_Build"
$vcpkgRoot = [IO.Path]::Combine($rootDir, 'vcpkg')
$vcpkgPath = [IO.Path]::Combine($vcpkgRoot, 'scripts', 'buildsystems', 'vcpkg.cmake')

$command = ". $tripleQuotes$cleanEnvScript$tripleQuotes; & $tripleQuotes$executable$tripleQuotes $argumentsWithEscapedQuotes"
$arg = "-G `"Visual Studio 17 2022`"", "-A", "x64", "-S `"$cmakeSourceDir`"", "-B `"$cmakeBuildDir`"", "-DCMAKE_TOOLCHAIN_FILE=$vcpkgPath", "-DOPTION_ENABLE_ALL_APPS=OFF"

$process = Start-Process cmake -ArgumentList $arg -PassThru -NoNewWindow