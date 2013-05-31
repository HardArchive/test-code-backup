/* 
 * FILE NAME:			common.js
 * FILE DESCRIPTION:	General purpose JavaScript functions.
 * AUTHOR:				Victor
 * DATE CREATED:		2004-01-16
 * DATE MODIFIED:		2004-01-21
 */

// DESCRIPTION:		Function to toggle the visibility of a content panel.
// INSTRUCTIONS:	Use this helper function to change the visibility of a
//					content block and image of the toggle button on the panel.
// INPUT:
//   objImage		the image object of the toggle button
//   objContent		object on which 'display' CSS property will be changed
//   strDir			[optional]path to where image files are stored
// RETURN:
//   N/A
function TogglePanel(objImage, objContent, strDir)
{
	if (strDir == null) strDir = "images/"; // Default store path for images
	if (objContent.style.display == "") {
		objContent.style.display = "none";
		if (objImage != null)
			objImage.src = strDir + "panel_hidden.gif";
	} else {
		objContent.style.display = "";
		if (objImage != null)
			objImage.src = strDir + "panel_visible.gif";
	}
}

// DESCRIPTION:		Function to load the article page in a popup window.
// INSTRUCTIONS:	N/A
// INPUT:
//  nID			ID of the article
// RETURN:
//   N/A
function ShowArticle(nID)
{
	window.open("show_article.php?id=" + nID, "_blank", "location=no,menubar=no,toolbar=no,status=yes,resizable=yes,scrollbars=no", true);
}

// DESCRIPTION:		Function to perform a RegExp replace to trim both of the
//					left and right part whitespace characters(including the
//					Chinese full-width whitespace character) from a string,
//					as well as replacing CR LF with LF.
// INSTRUCTIONS:	N/A
// INPUT:
//   String want to trim
// RETURN:
//   trimmed string
function PowerTrim(str)
{
	var reg = /(^(\s|¡¡)+)|((\s|¡¡)+$)/g;
	return str.replace("\r\n", "\n").replace(reg, "");
}

// DESCRIPTION:		Function to retrieve coordinates of an object relative to
//					the document root(absolute position).
// INSTRUCTIONS:	N/A
// INPUT:
//   The object which coordinates wish to retrieve.
// RETURN:
//   An array object with .left and .top properties set.
function GetPos(obj)
{
	var oResult = new Array;
	oResult.left = obj.offsetLeft;
	oResult.top = obj.offsetTop;
	while(obj.tagName.toUpperCase() != "BODY") {
		if (!(obj = obj.offsetParent))
			break;
		oResult.left += obj.offsetLeft;
		oResult.top += obj.offsetTop;
	}
	return oResult;
}
