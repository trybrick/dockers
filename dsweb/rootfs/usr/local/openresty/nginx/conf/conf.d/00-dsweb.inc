# misc
set $gdsapi     "clientapi-DS_ENV.brickinc.net";
set $gdscontent "cdn-DS_ENV.brickinc.net";
set $isbrickinc 0;
set $nocache    0;
set $disallow   "";
set $gmycb      ""; # cache buster
set $nowww      0;
set $prerender  0;
set $invaliduri 0;
set $httpsre    0;
set $myapi      "";
set $myurl      "";

set_by_lua $hostenv '
    local hostenv      = "."
    local host         = tostring(ngx.var.host)
    local user_agent   = tostring(ngx.var.http_user_agent)
    local prerender    = ngx.var.prerender
    local gmycb        = ngx.today()

    ngx.header["Server"] = nil
    ngx.header["X-UA-Compatible"] = "IE=Edge,chrome=1"

    if ngx.re.match(tostring(ngx.var.http_referer), [[(webvisor|trigs)+\\.com]]) == nil then
        ngx.header["X-Frame-Options"] = "SAMEORIGIN"
    end

    if ngx.re.match(tostring(ngx.var.uri), [[^[\\w\\-,. _~\\/]+$]]) == nil then
        ngx.var.invaliduri = 1
    end


    local regex        = [[\\.(\\w{3})\\.brickinc\\.net$]]
    local m            = ngx.re.match(host, regex)

    if m then
        hostenv = m[0]
        ngx.var.nowww = 0
        ngx.var.isbrickinc = 1
        ngx.var.nocache = 1
        ngx.var.disallow = "*"

        ngx.var.gdsapi = string.format("clientapi-%s.brickinc.net", hostenv)
        ngx.var.gdscontent = string.format("cdn-%s.brickinc.net", hostenv)

        gmycb = ngx.utctime()
        ngx.header["X-Frame-Options"] = nil

    else
        -- host with 1 period means no www    
        local _, count = string.gsub(host, "%.", "-")
        if (count == 1) then
            ngx.var.nowww = 1
        end

        -- ignore letsencrypt
        if string.find(ngx.var.request_uri, "known/acme") ~= nil then
            ngx.var.nowww = 0
        end

    end


    regex = "(Googlebot|yahoo|bingbot|yandex|yeti|yodaobot|gigabot|ia_archiver|baiduspider|twitterbot|facebookexternalhit|rogerbot|linkedinbot|embedly|quora link preview|showyoubot|outbrain|pinterest|slackbot|vkShare|W3C_Validator)"
    m = ngx.re.match(user_agent, regex)

    if m or string.find(tostring(ngx.var.args), "_escaped_fragment_") then
        prerender = 1
    end

    regex = [[\\.(js|css|xml|less|png|jpg|jpeg|gif|pdf|doc|txt|ico|rss|zip|mp3|rar|exe|wmv|doc|avi|ppt|mpg|mpeg|tif|wav|mov|psd|ai|xls|mp4|m4a|swf|dat|dmg|iso|flv|m4v|torrent|ttf|woff|svg|eot)]]
    m = ngx.re.match(ngx.var.uri, regex)

    if m or string.find(user_agent, "Prerender") then
        prerender = 0
        ngx.header["X-Frame-Options"] = nil
        ngx.header["X-UA-Compatible"] = nil
    end

    if prerender == 1 and ngx.var.scheme == "http" then
        ngx.var.httpsre = 1
    end

    ngx.var.prerender = prerender
    ngx.var.gmycb = gmycb
    return "."
';

limit_req zone=dsperip burst=600 nodelay;

if ($nowww = 1) {
    return 301 $scheme://www.$host$request_uri; 
}

if ($httpsre = 1) {
    return 301 https://$host$request_uri;
}

if ($invaliduri = 1) {
    return 403;
}

location /healthcheck {
    default_type text/plain;
    return 200 "OK";
}

location /robots.txt {
    default_type text/plain;
    return 200 "User-agent: *\nDisallow: $disallow\n";
}

location = /50x.html {
    root   /usr/local/openresty/nginx/html;
}

location = /404.html {
    root   /usr/local/openresty/nginx/html;
}

location /.well-known/apple-app-site-association {
    return 403;
}

location / {
    if ($prerender = 1) {
        set $myapi     "ds-seo.brickinc.net";
        set $myurl     "http://$myapi:4001/$scheme://$host$request_uri?";
        rewrite ^ /prerender last;
    }

    set $myapi         "$gdsapi";
    set $myurl         "http://$myapi/api/v1/content/getpage/?cb=$gmycb&scheme=$scheme&host=$host&request_uri=$request_uri";

    include            conf.d/proxy-hide-headers.inc;

    proxy_cache_valid  20m;
    proxy_set_header   Host $myapi;
    proxy_cache_bypass $nocache;
    proxy_no_cache     $nocache;
    proxy_pass         $myurl;
}

location /prerender {
    internal;

    include            conf.d/proxy-hide-headers.inc;

    proxy_cache_key    "$request_method$myurl";
    proxy_cache_valid  2h;
    proxy_set_header   Host $myapi;
    proxy_pass         $myurl;
}

# block access to sensitive files or a php file scan
location ~* \.(bak|conf|dist|fla|in[ci]|log|psd|sh|sql|sw[op]|php|cgi) {
    return 403;
}

# block access to sensitive path scan
location ~* ^/(cgi-sys|myadmin|wp-admin|sql|db|database|administrator|phpmyadmin\d*|pma\d*|mysql|php-my-admin|phpmanager)/ {
    return 403;
}

location ~* ^/view/(.*)\.html {
    return 404;
}

location ~* ^/(script|asset)/(.*) {
    proxy_cache_valid  20m;
    
    include            conf.d/proxy-hide-headers.inc;
    
    set $myurl         "$gdscontent";
    
    proxy_set_header   Host $myurl;
    proxy_pass         http://$myurl;
    proxy_cache_bypass $nocache;
    proxy_no_cache     $nocache;
}

location ~* ^/proxy/(.*) {
    proxy_hide_header  "X-AspNet-Version";
    proxy_hide_header  "X-Powered-By";

    set $myurl         "$gdsapi";

    proxy_set_header   Host $myurl;
    proxy_pass         http://$myurl/api/v1/$1$is_args$args;
    proxy_cache_bypass 1;
    proxy_no_cache     1;
}

