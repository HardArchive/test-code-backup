@echo 将所有文件复制到这个目录下 ：_IConv
pause
md _IConv
copy D:\WinPath\desktop\test_iconv\ZipDir\lib\* D:\WinPath\desktop\test_iconv\ProjectDir\_IConv\
copy D:\WinPath\desktop\test_iconv\ZipDir\include\IConv.h.in  D:\WinPath\desktop\test_iconv\ProjectDir\_IConv\IConv.h
copy D:\WinPath\desktop\test_iconv\ZipDir\srcLib\localcharset.h  D:\WinPath\desktop\test_iconv\ProjectDir\_IConv\
copy D:\WinPath\desktop\test_iconv\ZipDir\libcharset\lib\localcharset.c  D:\WinPath\desktop\test_iconv\ProjectDir\_IConv\
ren  D:\WinPath\desktop\test_iconv\ProjectDir\_IConv\Config.h.in Config.h
@echo 目录文件生成成功。
pause
