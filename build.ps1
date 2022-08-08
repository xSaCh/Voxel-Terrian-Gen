$func = $args[0]

#C++ Variables
$CXX_FLAGS     = "-std=c++17"
$SRC           = ""

foreach($i in ls src\*.cpp -Recurse)
{
    $SRC += $i.FullName + " "
}

$INCLUDE       = ".\includes"
$IMGUI_INCLUDE = ".\includes\imgui"
$LIB           = ".\lib"

$LIBRARIES      = "-lglad -lglfw3dll"
$EXECUTABLE     = "main"

function compile
{
    g++ $CXX_FLAGS $SRC -I $INCLUDE -L $LIB $LIBRARIES -o $EXECUTABLE
}

function build
{
    Write-Host "Build"
}

switch($func)
{
    "c" {compile}
    "b" {build}
}