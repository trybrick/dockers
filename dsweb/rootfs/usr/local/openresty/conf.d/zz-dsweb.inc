# misc

location /healthcheck {
  	default_type text/plain;
	return 200 "OK";
}

set $prerender 0;
if ($http_user_agent ~* "googlebot|yahoo|bingbot|yandex|yeti|yodaobot|gigabot|ia_archiver|baiduspider|twitterbot|facebookexternalhit|rogerbot|linkedinbot|embedly|quora link preview|showyoubot|outbrain|pinterest|slackbot|vkShare|W3C_Validator") {
    set $prerender 1;
}

if ($args ~ "_escaped_fragment_") {
    set $prerender 1;
}

if ($http_user_agent ~ "Prerender") {
    set $prerender 0;
}

if ($uri ~* "\.(js|css|xml|less|png|jpg|jpeg|gif|pdf|doc|txt|ico|rss|zip|mp3|rar|exe|wmv|doc|avi|ppt|mpg|mpeg|tif|wav|mov|psd|ai|xls|mp4|m4a|swf|dat|dmg|iso|flv|m4v|torrent|ttf|woff|svg|eot)") {
    set $prerender 0;
}

if ($prerender = 1) {
    # setting prerender as a variable forces DNS resolution since nginx caches IPs and doesnt play well with load balancing
    set $prerender "DS_SEO";
    rewrite .* /$scheme://$host$request_uri? break;
    proxy_pass http://$prerender:4001;
}

error_page  404              /404.html;

# redirect server error pages to the static page /50x.html
#
error_page  500 502 503 504  /50x.html;

location = /50x.html {
    root   /usr/local/openresty/nginx/html;
}