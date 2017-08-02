# detect non-www host
set $not_www_host 0;

if ( $host !~ "^www" ) {
	set $not_www_host 1;
}

# exclude brickinc domain from non-www host
if ( $isbrickinc = 1 ) {
	set $not_www_host 0;
}

# exclude letsencrypt challenge
if ( $request_uri ~ (.*)/acme-challenge/(.*) ) {
	set $not_www_host 0;
}

if ( $not_www_host = 1 ) {
	return 301 $scheme://www.$host$request_uri; 
}