for %%i in (*.proto) do (  
    protoc.exe  --cpp_out=../../libs/pb/src %%i      
) 
pause