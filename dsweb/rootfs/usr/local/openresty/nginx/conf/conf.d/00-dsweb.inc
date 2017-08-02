# misc

location /healthcheck {
    default_type text/plain;
	return 200 "OK";
}

location / {
    include /usr/local/openresty/nginx/conf/conf.d/proxy-hide-headers.inc;

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

    if ($http_referer !~* "(webvisor\.com|trigs\.com)") {
        add_header X-Frame-Options "SAMEORIGIN";
    }

    set $myapi "DS_API";
    set $myurl "http://$myapi/api/v1/content/getpage/?host=$host&request_uri=$request_uri";
    proxy_set_header Host $myapi;
    proxy_pass $myurl;
}

location = /50x.html {
    root   /usr/local/openresty/nginx/html;
}

location = /404.html {
    root   /usr/local/openresty/nginx/html;
}

location ~ ^/asset/(.*) {
    include /usr/local/openresty/nginx/conf/conf.d/proxy-hide-headers.inc;
    set $myurl "DS_CONTENT";
    proxy_set_header Host $myurl;
    proxy_pass http://$myurl;
}

location ~ ^/script/(.*) {
    include /usr/local/openresty/nginx/conf/conf.d/proxy-hide-headers.inc;
    set $myurl "DS_CONTENT";
    proxy_set_header Host $myurl;
    proxy_pass http://$myurl;
}

location ~ ^/proxy/(.*) {
    proxy_hide_header           "Server";
    proxy_hide_header           "X-AspNet-Version";
    proxy_hide_header           "X-Powered-By";

    set $myurl "DS_API";

    proxy_set_header   Host $myurl;
    proxy_pass         http://$myurl/api/v1/$1$is_args$args;
    proxy_cache_bypass 1;
    proxy_no_cache     1;
}
