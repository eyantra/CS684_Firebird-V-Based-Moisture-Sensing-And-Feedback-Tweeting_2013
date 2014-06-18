<?php
include_once 'tweet.php';
$locations = unserialize($_COOKIE['areas']);
$i = 0;
while(isset($locations[$i]['sensor_val']))
	$i++;
if($i == count($locations))
{
	if(file_exists("senseDone.txt"))
	{
		for($j=0; $j < $i; $j++)
		{
			if($locations[$j]['trough'] == "1" || $locations[$j]['trough'] == "2" )
			{
				if($locations[$j]['water'] == 1)
				{
					exec("./startWater &");
				}
			}
			if($locations[$j]['water'] == 1)
			{
				tweet("Water low in area".$locations[$j]['trough']." on ".date('Y-m-d h:i:s',time()));
			}
		}
		$fout = fopen("log.csv","a");
		for($j=0;$j<$i;$j++)
		{
			fputcsv($fout, array(date('Y-m-d h:i:s',time()),$locations[$j]['trough'], $locations[$j]['distance'], $locations[$j]['sensor_val']));
		}
		fclose($fout);
		unlink("senseDone.txt");
		echo "done";
	}
	else
	{
		echo "not done";
	}
}
else
{
	if(file_exists("sensed".($i+1).".csv"))
	{
		$file = fopen("sensed".($i+1).".csv","r");
		$locations[$i]['sensor_val'] = fgets($file);
		if($locations[$i]['sensor_val'] < 150) {
			$locations[$i]['water'] = 1;
		}
		else
		{
			$locations[$i]['water'] = 0;
		}
		fclose($file);
		unlink("sensed".($i+1).".csv");
		setcookie("areas", serialize($locations));
		$i++; 
		if($i == count($locations))
		{
			//echo "done";
			unlink("sense.txt");
		}
		else
		{
			//echo "not done";
		}
		echo ". read ".$locations[$i-1]['trough'].":".$locations[$i-1]['distance'].":".$locations[$i-1]['sensor_val'].":".$locations[$i-1]['water'];
	}
	else
	{
		echo "not done";
	}
}
?>
