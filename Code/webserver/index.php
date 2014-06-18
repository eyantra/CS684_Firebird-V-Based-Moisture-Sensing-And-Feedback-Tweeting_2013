<html>
<head>
<script src="js/jquery.js">
</script>
<script language="javascript">
var poller;
function do_this()
{
	$.ajax({
		type:"GET",
		url:"tr2.php",
		success:function(response){
			if(response == 'done')
			{
		        	clearInterval(poller);
				$('#sense').removeAttr('disabled');
			}
			else
			{
				if(/read/.test(response))
				{
					var cp = response.match(/[0-9]+\:[0-9]+\:[0-9]+\:[0-9]+/);
					var vals = cp[0].split(":");
					var new1 = $("<div></div>");
					$(new1).addClass('reading');
					$(new1).html("r:"+vals[2]);
					var x = $('#a'+vals[0]).offset();
					$(new1).css({top: (x.top + (parseInt(vals[1])) * 2), left: (x.left+5), position:'absolute'});
					if(vals[3] == '1')
					{
						$('#a'+vals[0]).css('background-color','#FF0000');
					}
					else
					{
						$('#a'+vals[0]).css('background-color','#00FF00');
					}
					$('body').append(new1);
				}
			}
		}
	});
}

$(document).ready(function(){
	<?php
		if(file_exists('sense.txt'))
		{
			echo '$("#sense").attr("disabled","disabled");';
		}
	?>
	$('#trough tr td').click(function(e){
		if($(this).hasClass('lane') || $(this).hasClass('start') || $(this).hasClass('empty'))
		{
			return;
		}
		var offset = $(this).offset();
		var tno=parseInt($(this).attr('id').substring(1,2));
		var loc;
		loc = Math.round((e.clientY - offset.top)/2);
		var trow = $('<tr></tr>');
		$(trow).html("<td>"+$(this).attr('id')+"</td><td>"+loc+" cm</td>");
		$(trow).append("<td><a href='javascript:void(0)' onclick='this.parentNode.parentNode.parentNode.removeChild(this.parentNode.parentNode);'>X</a></td>");
		$('#selEl').append(trow);
	});
	$('#sense').click(function(){
		var areas="";
		if($('#selEl tr').length == 0)
		{
			alert("Select a location");
			return;
		}
		$('#selEl tr').each(function(value){
			childtr = $(this).children('td');
			areas+='&trough[]='+$(childtr[0]).html().substring(1,2)+'&distance[]='+$(childtr[1]).html().substring(0,$(childtr[1]).html().length-3);
		});
		$('#sense').attr('disabled','disabled');
		$.ajax({
			type:"POST",
			url:"trial.php",
			data: areas.substring(1,areas.length),
			success:function(response){
				$('#resp').html(response);
				$('#selEl').html('');
				poller = setInterval(do_this,3000);
			}
		});
	});
});
</script>
<title>
Welcome to Plant Watering System
</title>
<style>
#trough tr td {
	padding:5px;
	border:1px solid;
	background-color:#F2B34E;
}
#trough tr td.lane {
	padding:25px;
	background-color:#F7ECB7; /*#F4F0CB;*/
}
#trough tr td.start {
	padding:15px;
	background-color:#F2B34E;
}
#trough tr td.empty {
	background-color:#FFFFFF;
}
</style>
</head>
<body>
<center><h1>Moisture Sensing and Feedback Tweeting Bot</h1></center>
<div id="resp"></div>
<div style="float:left;width:70%">
<p>Below is the grid that represents the layout of the Green House.
Select the location where you want to sense the moisture level by clicking on the appropriate location</p>
<br />
<center>
<table id="trough">
	<tr>
		<td class="start">Start Point</td>
		<td rowspan="3" class="lane"></td>
		<td colspan="2" class="lane"></td>
		<td rowspan="3" class="lane"></td>
		<td colspan="2" class="lane"></td>
		<td rowspan="3" class="lane"></td>
		<td colspan="2" class="lane"></td>
		<td rowspan="3" class="lane"></td>
	</tr>
	<tr>
		<td rowspan="2" class="empty"></td>
		<td id="a1" style="height:220px">
		Area1
		</td>
		<td id="a2">
		Area2
		</td>
		<td id="a3">
		Area3
		</td>
		<td id="a4">
		Area4
		</td>
		<td id="a5">
		Area5
		</td>
		<td id="a6">
		Area6
		</td>
	</tr>
	<tr>
		<td colspan="2" class="lane"></td>
		<td colspan="2" class="lane"></td>
		<td colspan="2" class="lane"></td>
	</tr>
</table>
<br />
<table id="selEl"></table>
<br />
<input type="button" id="sense" value="Start Sensing" /></center>
<br />
</div>
<div style="float:right;width:30%">
<a style="float:right" href="readLog.php">View Sensor Readings Log</a>
<br />
<center><h2>Tweet Monitor</h2></center>
<a class="twitter-timeline" href="https://twitter.com/IITBTweetbot"
data-widget-id="399600960302874624">Tweets by @IITBTweetbot</a>
<script>!function(d,s,id){var
js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?'http':'https';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+"://platform.twitter.com/widgets.js";fjs.parentNode.insertBefore(js,fjs);}}(document,"script","twitter-wjs");</script>
</div>
</body>
</html>
