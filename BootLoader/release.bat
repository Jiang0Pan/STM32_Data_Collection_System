@ ���Ը�����Ҫ�޸Ķ�Ӧ��keil������
set project=boot

@ ɾ���ɰ汾��release��
del  /y ..\release_boot

@ ����release�ļ���
mkdir ..\release_boot
mkdir ..\release_boot\MDK-ARM
mkdir ..\release_boot\MDK-ARM\boot

@ ������������ļ�
copy .\MDK-ARM\%project%.uvprojx  ..\release_boot\MDK-ARM
copy .\MDK-ARM\%project%.uvoptx   ..\release_boot\MDK-ARM
copy .\MDK-ARM\boot\%project%.axf  ..\release_boot\MDK-ARM\boot

exit