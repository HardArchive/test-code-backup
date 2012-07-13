xmlHead = '<?xml version="1.0" encoding="utf-8" ?>\n'

-- Open note file to wriet.  通过IO库操作文件的
function openNoteFile(fileName)
	return io.open(fileName, "w")
end


-- Close writed note file.
function closeNoteFile(noteFile)
	noteFile:close()
end

-- writeNestedLabel函数根据当前XML的缩进数目来规范XML输出格式
function writeNestedLabel(ioChanel, label, nestCnt)
	if nestCnt == 0 then
		ioChanel:write(label)
		return
	end

	for i = 1, nestCnt do
		ioChanel:write("\t")
	end

	ioChanel:write(label)
end

-- generateNoteXML全局函数接受发送方姓名、接收方姓名、便签内容，生成一个XML便签文件
function generateNoteXML(fromName, toName, msgContent)
	local noteFile = openNoteFile(fromName .. "_" .. toName .. ".xml")
	if not noteFile then
		return false
	end

	noteFile:write(xmlHead)
	noteFile:write("<note>\n")

	local currNestCnt = 1
	writeNestedLabel(noteFile, "<fromName>", currNestCnt)
	noteFile:write(fromName)
	writeNestedLabel(noteFile, "</fromName>\n", 0)

	writeNestedLabel(noteFile, "<toName>", currNestCnt)
	noteFile:write(toName)
	writeNestedLabel(noteFile, "</toName>\n", 0)

	local sendTime = os.time()   --Lua标准库os.time()函数获取发送时间
	writeNestedLabel(noteFile, "<sendTime>", currNestCnt)
	noteFile:write(sendTime)
	writeNestedLabel(noteFile, "</sendTime>\n", 0)

	writeNestedLabel(noteFile, "<msgContent>", currNestCnt)
	noteFile:write(msgContent)
	writeNestedLabel(noteFile, "</msgContent>\n", 0)

	noteFile:write("</note>\n")
	closeNoteFile(noteFile)

	return true
end