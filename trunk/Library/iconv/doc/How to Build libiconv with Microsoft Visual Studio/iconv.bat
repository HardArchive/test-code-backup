@echo �������ļ����Ƶ����Ŀ¼�� ��_IConv
pause
md .\_IConv
copy .\libiconv-1.14\lib\*  .\_IConv\
copy .\libiconv-1.14\include\IConv.h.in  .\_IConv\IConv.h
copy .\libiconv-1.14\srcLib\localcharset.h  .\_IConv\

copy .\libiconv-1.14\libcharset\include\localcharset.h.in  .\_IConv\
copy .\libiconv-1.14\libcharset\lib\localcharset.c  .\_IConv\
ren  .\_IConv\Config.h.in Config.h

ren  .\_IConv\localcharset.h.in  localcharset.h

@echo Ŀ¼�ļ����ɳɹ���
pause