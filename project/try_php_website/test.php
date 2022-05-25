<!DOCTYPE html>
<html lang="en" >
<head>
    <meta charset="UTF-8">
    <title>Post a form</title>
    <link rel="stylesheet" href="style.css">
</head>

<body>
    <div class="full-screen">
    <h2>   
        Hello 
        <?php 
            echo $_POST["firstname"] . " " . $_POST["lastname"];            
        ?>
        You want to tell us: 
        <?php
            echo $_POST["subject"];
        ?> 
    </h2>
    </div>

</body>
</html>