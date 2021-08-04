@ 可以根据需要修改对应的keil工程名
set project=DataCollectSoftware

@ 删除旧版本的release包
del /y  ..\release_app

@ 创建release文件包
mkdir ..\release_app
mkdir ..\release_app\MDK-ARM
mkdir ..\release_app\MDK-ARM\%project%

@ 拷贝相关升级文件
copy .\MDK-ARM\%project%.uvprojx  ..\release_app\MDK-ARM
copy .\MDK-ARM\%project%.uvoptx   ..\release_app\MDK-ARM
copy .\MDK-ARM\%project%\%project%.axf  ..\release_app\MDK-ARM\%project%
copy .\MDK-ARM\%project%\%project%.bin  ..\release_app\

@ 退出
exit