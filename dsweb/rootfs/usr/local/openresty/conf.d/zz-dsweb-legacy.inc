add_header X-UA-Compatible "IE=edge";

# detect non-www host
set $not_www_host 0;

if ($host !~ "^www") {
	set $not_www_host 1;
}

# exclude brickinc domain from non-www host
if ($http_host ~ “\.brickinc\.net$”) {
	set $not_www_host 0;
}

if ($not_www_host = 1) {
	return 301 $scheme://www.$host$request_uri; 
}

# expresslane redirects
location ~* ^/default.aspx(.*) {
	return 301 /;
}

location ~* shop/weeklyad {
	return 301 /circular;
}

location ~* shop/coupon {
	return 301 /coupons;
}

location ~* shop/managelist {
	return 301 /mylist;
}

location ~* ^/recipes/(.*) {
	return 301 /recipecenter;
}

if ($http_referer !~* "(webvisor\.com|trigs\.com)") {
	add_header X-Frame-Options "SAMEORIGIN";
}
