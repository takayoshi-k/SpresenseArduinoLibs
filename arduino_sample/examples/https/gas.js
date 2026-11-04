// Googleドライブの保存先フォルダID
const FOLDER_ID = "1M_of_4wArUF1qnRsll7WOMg8xqYHsTij";

function doPost(e) {
  let step = 0;
  let mimeType = "";
  try {
    // 1. 保存先のフォルダIDを指定[cite: 1]
    const folderId = FOLDER_ID;
    const folder = DriveApp.getFolderById(folderId);
    step = 1;

    // 2. POSTリクエストのボディ（Base64文字列）を取得[cite: 1]
    const postData = e.postData;
    step = 2;
    const contents = postData.contents;
    step = 3; 
    mimeType = postData.type;
    step = 4;

    // 3. クエリパラメータからファイル名を取得[cite: 1]
    const fileName = e.parameter.filename || "uploaded_file";

    step = 5;

    let mime;
    let uploadedFile;

    if (mimeType === "text/plain") {
      mime = "Plane Text";
      const blob = Utilities.newBlob(contents, mimeType, fileName);
      uploadedFile = folder.createFile(blob);
      step = 6;
    } else if (mimeType === "image/jpeg" || mimeType === "image/png") {
      mime = "Image File";
      // 4. Base64文字列をデコードしてBlobに変換し、ファイルを作成[cite: 1]
      const decodedBytes = Utilities.base64Decode(contents);
      const blob = Utilities.newBlob(decodedBytes, mimeType, fileName);
      uploadedFile = folder.createFile(blob);
      step = 7;
    } else {
      mime = "Unknown";
      step = 8;
    }

    // 5. 成功時のレスポンスを返す[cite: 1]
    return ContentService.createTextOutput(JSON.stringify({
      status: "success",
      message: "アップロード成功（Base64）",
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
