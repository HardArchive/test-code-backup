set ws=WScript.CreateObject("WScript.Shell") 
ws.Run "regsvr32 /u COMRegister.dll", 0
