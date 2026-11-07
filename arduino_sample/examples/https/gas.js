// Googleドライブの保存先フォルダID
const FOLDER_ID = "1M_of_4wArUF1qnRsll7WOMg8xqYHsTij";

function doPost(e) {
  let mimeType = "";
  try {
    const folderId = FOLDER_ID;
    const folder = DriveApp.getFolderById(folderId);

    const postData = e.postData;
    const contents = postData.contents;
    mimeType = postData.type;

    const fileName = e.parameter.filename || "uploaded_file";

    let uploadedFile;

    if (mimeType === "text/plain") {
      const blob = Utilities.newBlob(contents, mimeType, fileName);
      uploadedFile = folder.createFile(blob);
    } else if (mimeType === "image/jpeg" || mimeType === "image/png") {
      const decodedBytes = Utilities.base64Decode(contents);
      const blob = Utilities.newBlob(decodedBytes, mimeType, fileName);
      uploadedFile = folder.createFile(blob);
    } else {
    }

    return ContentService.createTextOutput(JSON.stringify({
      status: "success",
      message: "Upload succeeded",
      fileUrl: uploadedFile.getUrl(),
      fileName: uploadedFile.getName(),
      size: uploadedFile.getSize(),
      type: mimeType
    })).setMimeType(ContentService.MimeType.JSON);
  } catch (error) {
    return ContentService.createTextOutput(JSON.stringify({
      status: "error",
      message: error.toString(),
      step : step,
      type : mimeType
    })).setMimeType(ContentService.MimeType.JSON);
  }
}

function doGet(e) {
  return ContentService.createTextOutput("GAS WebApp is running. Use POST to upload Base64 encoded files.");
}
