<?php
session_start();
$link = pg_connect("host=labdb dbname=bd user=mm395135 password=KaczyKonus2609") or die ("Could not connect to server\n");
if(isset($_SESSION['login'])){
	header('Location: kasjer.php');
	exit;
}

if(isset($_POST['login'])){

	$hasło_qry = pg_query_params($link, "SELECT hasło FROM Kasjer WHERE login = $1", array($_POST['login']));
	if(pg_num_rows($hasło_qry) == 0){
		$error = "Nie ma takiego użytkownika.";
	}
	else{
		$hasło = pg_fetch_array($hasło_qry, 0);
		if($hasło["hasło"] == $_POST['password']){
			$_SESSION['login'] = $_POST['login'];
			header('Location: kasjer.php');
			exit;
		} else {
			$error = "Hasło niepoprawne.";
		}
	}	
}
?>

<html>
<head>
    <title>Kino Lunar</title>
	<link rel="stylesheet" type="text/css" href="style.css"/>
</head>
<body>

<div class="header">
  <a href="https://students.mimuw.edu.pl/~mm395135/bd/kino.php"> <img src="lunar.png" alt="Lunar" width="400" height="175"> </a>
</div>


<a href="repertuar.php"> <button class="button"> Repertuar </button> </a>
<a href="sprawdz_rezerwacje.php"> <button class="button"> Sprawdź rezerwację </button> </a>
<a href="logowanie.php"> <button class="button"> Panel kasjera </button> </a>

<h3> Logowanie </h3>
<form action="logowanie.php" method="post">
  <p>
    <label for="login">Login:</label>
    <input id="login" name="login" type="text" required>
  </p>
  <p>
    <label for="password">Hasło:</label>
    <input id="password" name="password" type="password" required>
  </p>
  <p>
    <button class='button button2' type='submit' value='submit'>Zaloguj</button> <br>
  </p>
   <p><font style="color:red;"><?php echo $error;?></font></p> 

</form>
    
</body>
</html>
