<?php

function sortfunc($a, $b)
{
    if (($a['trough'] == $b['trough'])
        || ((abs($a['trough'] - $b['trough']) == 1) 
        && (((($a['trough'] - 1) % 4) + (($b['trough'] - 1) % 4)) == 3)))
    {
	if ($a['distance'] == $b['distance'])
            return 0;
	elseif ($a['distance'] < $b['distance'])
            return -1;
	else
            return 1;
    }
    else 
    {
        return ($a['trough'] < $b['trough']) ? -1 : 1;
    }
}

$fp = fopen('sense.txt', 'a+');
fclose($fp);

$entries = count($_POST['trough']);

for($i = 0; $i < $entries; $i++)
{
	$locations[$i]['trough'] = $_POST['trough'][$i];
	$locations[$i]['distance'] = $_POST['distance'][$i];
	if ($locations[$i]['distance'] > 55) {
		$locations[$i]['checkpt'] = 2;
		$locations[$i]['offset'] = $locations[$i]['distance'] - 55;
	}
	else {
		$locations[$i]['checkpt'] = 1;
		$locations[$i]['offset'] = $locations[$i]['distance'];
	}
	$locations[$i]['linept'] = floor($locations[$i]['trough']/2) + 1;
	$locations[$i]['direction'] = (($locations[$i]['trough'] % 2) == 0)?'r':'l';
}
usort($locations,'sortfunc');

$fp = fopen('locations.csv', 'w');

for($i = 0; $i < count($locations); $i++)
{
	fwrite($fp,$locations[$i]['linept'].",".$locations[$i]['checkpt'].",".$locations[$i]['offset'].",".$locations[$i]['direction']."\n");
}
fwrite($fp,"0");
fclose($fp);

exec('./startSensing > /dev/null 2>&1 &');

setcookie ("areas",serialize($locations));
?>
