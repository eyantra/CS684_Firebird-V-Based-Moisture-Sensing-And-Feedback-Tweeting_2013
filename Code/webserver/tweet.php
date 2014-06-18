<?php
require_once('TwitterAPIExchange.php');

/** Set access tokens here - see: https://dev.twitter.com/apps/ **/
$settings = array(
    'oauth_access_token' => "1962403735-pMp0Y3we6hGknvgvHFlzarDuUmLOqXXzfdKSOP7",
    'oauth_access_token_secret' => "q26HJn1Tnz4YtQqyHmJbxPxXZK4b5mtIRxkDNcDE",
    'consumer_key' => "JDsBC5u6HepMK2BT72Gw",
    'consumer_secret' => "wqo7UiL0oGcm35r3fZPz9TCtCYO7nSUwGPUHJj5Ts"
);

function tweet($string)
{
global $settings;
$url = 'https://api.twitter.com/1.1/statuses/update.json'; 
$requestMethod = 'POST';
$postfields = array('status' => $string ); 
$twitter = new TwitterAPIExchange($settings);
$twitter->buildOauth($url, $requestMethod)
             ->setPostfields($postfields)
             ->performRequest();
}
