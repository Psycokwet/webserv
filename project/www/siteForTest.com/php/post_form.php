<!DOCTYPE html>
<html lang="en" >
<head>
    <meta charset="UTF-8">
    <title>Post a form</title>
    <link rel="stylesheet" href="../css/style.css">
</head>

<body>
    <div class="full-screen">
        <h3>
            Hello
            <?php 
                echo $_POST["firstname"] . " " . $_POST["lastname"];            
            ?>
            <br>
            You said: 
            <?php
                echo $_POST["subject"];
            ?> 
        </h3>
    </div>

</body>
</html>