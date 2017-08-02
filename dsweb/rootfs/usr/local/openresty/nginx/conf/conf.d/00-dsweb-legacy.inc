# expresslane redirects
location ~* ^/default.aspx(.*) {
	return 301 $scheme://$host/;
}

location ~* ^/shop/week {
	return 301 $scheme://$host/circular;
}

location ~* ^/shop/coupon {
	return 301 $scheme://$host/coupons;
}

location ~* ^/shop/managelist {
	return 301 $scheme://$host/mylist;
}

location ~* ^/recipes/(.*) {
	return 301 $scheme://$host/recipecenter;
}
