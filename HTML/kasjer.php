<?php
session_start();

if(!isset($_SESSION['login'])){
	header('Location: logowanie.php');
}
if(isset($_POST['logout']))
{
 unset($_SESSION['login']);
 header('Location: logowanie.php');
}
?>

<html>
<head>
<link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>

<div class="header">
  <a href="https://students.mimuw.edu.pl/~mm395135/bd/kino.php"> <img src="lunar.png" alt="Lunar" width="400" height="175"> </a>
</div>


<a href="repertuar.php"> <button class="button"> Repertuar </button> </a>
<a href="sprawdz_rezerwacje.php"> <button class="button"> Sprawdź rezerwację </button> </a>
<a href="logowanie.php"> <button class="button"> Panel kasjera </button> </a>


<h3>Kasjer</h3>

<form action="pokaz_rezerwacje.php" method='post'>
  <p>
    <button class='button button2' type='submit' name='rezerwacje' value='rezerwacje'>Sprawdź rezerwacje</button> <br>
  </p>
</form>
<form action="kasjer.php" method='post'>
  <p>
    <button class='button button2' type='submit' name='logout' value='logout'>Wyloguj</button>
  </p>
</form>

</body>
</html>
