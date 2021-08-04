@ 可以根据需要修改对应的keil工程名
set project=boot

@ 删除旧版本的release包
del  /y ..\release_boot

@ 创建release文件包
mkdir ..\release_boot
mkdir ..\release_boot\MDK-ARM
mkdir ..\release_boot\MDK-ARM\boot

@ 拷贝相关升级文件
copy .\MDK-ARM\%project%.uvprojx  ..\release_boot\MDK-ARM
copy .\MDK-ARM\%project%.uvoptx   ..\release_boot\MDK-ARM
copy .\MDK-ARM\boot\%project%.axf  ..\release_boot\MDK-ARM\boot

exit