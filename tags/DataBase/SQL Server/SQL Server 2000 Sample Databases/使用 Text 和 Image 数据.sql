USE pubs
DECLARE @val varbinary(16)
SELECT @val = TEXTPTR(logo) FROM pub_info
WHERE pub_id = '0736'
READTEXT pub_info.logo @val 4 10
 
