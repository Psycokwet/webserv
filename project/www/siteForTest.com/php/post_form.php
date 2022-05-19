<!DOCTYPE html>
<html lang="en" >
<head>
    <meta charset="UTF-8">
    <title>Post a form</title>
    <link rel="stylesheet" href="css/style.css">
</head>

<body>
    <div class="full-screen">
        Welcome
        <?php 
            echo "Hello";
            echo $_POST["firstname"] . " " . $_POST["lastname"];            
        ?>
        You want to tell us: 
        <?php
            echo $_POST["subject"];
        ?> 
    </div>

</body>
</html>