REM Start
w:
cd w:\boomworks\SrcCounter

copy /Y src\bin\release\*.exe release\sourcecounter\
xcopy /Y /f /s src\img\*.* release\sourcecounter\img\
xcopy /Y /f /s src\locales\*.mo release\sourcecounter\locales\
xcopy /Y /f /s src\locales\*.po release\sourcecounter\locales\

cd release\
copy /Y *.dll sourcecounter\
copy /Y *.txt sourcecounter\

REM copy zip to old verison folder
copy /Y *.zip oldversion\
copy /Y *.7z oldversion\

delete 7z zip files
del *.7z *.zip

REM 7z a -t7z Files.7z *.txt -r
7z a -t7z sourcecounter-2.1.16.89.7z SourceCounter\ -mx7
REM zip
7z a -tzip sourcecounter-2.1.16.89.zip SourceCounter\ -mx9

REM End