<html>
<head>
<style>
table tr td, table tr th {
	padding:2px;
	border:1px solid;
}
</style>
</head>
<body>
<?php
$fin = fopen("log.csv","r");
?>
<center>
<h1>Data Logged </h1>
<table style="border:1px solid">
<tr>
<th>TimeStamp</th>
<th>Area</th>
<th>Location</th>
<th>Sensed Value</th>
</tr>
<?php
while(($data = fgetcsv($fin)) !== FALSE)
{
	echo "<tr>";
	echo "<td>".$data[0]."</td>";
	echo "<td>".$data[1]."</td>";
	echo "<td>".$data[2]."</td>";
	echo "<td>".$data[3]."</td>";
	echo "</tr>";
}
?>
</table>
</center>
</body>
</html>
