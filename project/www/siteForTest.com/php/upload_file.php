<!DOCTYPE html>
<html lang="en" >
<head>
  <meta charset="UTF-8">
  <title>upload</title>
  <link rel="stylesheet" href="css/style.css">
</head>

<body>
<div class="full-screen">
    <div>
        <h2>
            <?php
                $target_dir = "/tmp/server_eval/uploads_dir/";
                $target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
                $imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));
                if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) 
                {
                    echo "The file ". htmlspecialchars( basename( $_FILES["fileToUpload"]["name"])). " has been uploaded.";
                } 
                else 
                {
                    echo "Sorry, there was an error uploading your file.";
                }
            ?>
        </h2>
    </div>
</div>
</body>

</html>