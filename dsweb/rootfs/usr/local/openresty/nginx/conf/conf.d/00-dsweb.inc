# misc
set $gdsseo     "DS_SEO";
set $gdsapi     "DS_API";
set $gdscontent "DS_CONTENT";
set $isbrickinc 0;
set $genv       ".";

# if host is brickinc
if ($host ~* (.*)\.brickinc\.net$ ) {
    set $isbrickinc  1;
    set $genv        $1;

    set_by_lua $genv '
        local genv = string.sub(ngx.var.genv, -3)
        local regex = "(dev|bta|tst|uat|stg)"
        local m = ngx.re.match(genv, regex)
        
        if m then
            ngx.var.gdsapi = string.format("clientapi-%s.brickinc.net", genv)
            ngx.var.gdscontent = string.format("cdn-%s.brickinc.net", genv)
        end

        return genv
    ';
}

location /healthcheck {
    default_type text/plain;
	return 200 "OK";
}

location / {
    include /usr/local/openresty/nginx/conf/conf.d/proxy-hide-headers.inc;
    include /usr/local/openresty/nginx/conf/conf.d/00-dsweb-www.inc;

    if ($http_referer !~* "(webvisor\.com|trigs\.com)") {
        add_header X-Frame-Options "SAMEORIGIN";
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

    set_by_lua $today 'return ngx.today()';
    set $myapi "$gdsapi";
    set $myurl "http://$myapi/api/v1/content/getpage/?cb=$today&scheme=$scheme&host=$host&request_uri=$request_uri";

    if ($prerender = 1) {
        set $myapi "$gdsseo";
        set $myurl "http://$myapi:4001/$scheme://$host$request_uri?";
    }

    proxy_cache_valid  DS_TTL;
    proxy_set_header   Host $myapi;
    proxy_cache_bypass $prerender;
    proxy_no_cache     $prerender;
    proxy_pass         $myurl;
}

location = /50x.html {
    root   /usr/local/openresty/nginx/html;
}

location = /404.html {
    root   /usr/local/openresty/nginx/html;
}

location ~ ^/asset/(.*) {
    proxy_cache_valid  DS_TTL;

    include /usr/local/openresty/nginx/conf/conf.d/proxy-hide-headers.inc;
    
    set $myurl "$gdscontent";
    proxy_set_header Host $myurl;
    proxy_pass http://$myurl;
}

location ~ ^/script/(.*) {
    proxy_cache_valid  DS_TTL;
    
    include /usr/local/openresty/nginx/conf/conf.d/proxy-hide-headers.inc;
    
    set $myurl "$gdscontent";
    
    proxy_set_header Host $myurl;
    proxy_pass http://$myurl;
}

location ~ ^/proxy/(.*) {
    proxy_hide_header           "Server";
    proxy_hide_header           "X-AspNet-Version";
    proxy_hide_header           "X-Powered-By";

    set $myurl "$gdsapi";

    proxy_set_header   Host $myurl;
    proxy_pass         http://$myurl/api/v1/$1$is_args$args;
    proxy_cache_bypass 1;
    proxy_no_cache     1;
}
