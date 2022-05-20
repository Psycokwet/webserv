<!DOCTYPE html>
<html lang="en" >
<head>
  <meta charset="UTF-8">
  <title>upload</title>
  <link rel="stylesheet" href="../css/style.css">
</head>

<body>
<div class="full-screen">
    <div>
        <h3>
            <?php
                $target_dir = "../uploads/";
                $target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
                $uploadOk = 1;
                $imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));
                // check if image file is a actual image or fake image
                if (isset($_POST["submit"]))
                {
                    $check = getimagesize($_FILES["fileToUpload"]["tmp_name"]);
                    if ($check !== false)
                    {
                        $uploadOk = 1;
                    }
                    else
                    {
                        echo "File is not an image.\n";
                        $uploadOk = 0;
                    }
                }
                // Check if file already exists
                if (file_exists($target_file))
                {
                    echo "File already exists.\n";
                    $uploadOk = 0;
                }
                // Check file size
                if ($_FILES["fileToUpload"]["size"] > 500000)
                {
                    echo "Your file is too large.\n";
                    $uploadOk = 0;
                }
                // Allow certain file formats
                if($imageFileType != "jpg" && $imageFileType != "png" && $imageFileType != "jpeg" && $imageFileType != "gif" ) 
                {
                    echo "Only JPG, JPEG, PNG & GIF files are allowed.\n";
                    $uploadOk = 0;
                }
                // Check if $uploadOk is set to 0 by an error
                if ($uploadOk == 0)
                {
                    echo "Sorry, your file was not uploaded.\n";
                }
                else // if everything is ok, try to upload file
                {
                    if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) 
                    {
                        echo "The file ". htmlspecialchars( basename( $_FILES["fileToUpload"]["name"])). " has been uploaded.\n";
                    } 
                    else 
                    {
                        echo "Sorry, there was an error uploading your file.\n";
                    }
                }
            ?>
        </h3>
    </div>
</div>
</body>

</html>