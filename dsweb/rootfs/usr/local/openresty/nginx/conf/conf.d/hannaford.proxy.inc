    location ~* ^/(Get|assets) {
        expires 1d;
        proxy_cache_lock             on;
        proxy_cache_lock_timeout     60s;

        # ProxySettings

        set $backend "hannaford-origin.brickinc.net";
        
        proxy_cache_key              hannaford$uri$is_args$args;
        proxy_set_header             Host "hannaford.gsnrecipes.com";
        proxy_ignore_headers         Set-Cookie;
        proxy_ignore_headers         Vary;
        proxy_ignore_headers         Expires;
        proxy_ignore_headers         Cache-Control;

        include                      conf.d/proxy-hide-headers.inc;

        proxy_set_header             X-Forwarded-For $remote_addr;
        add_header                   X-Cache $upstream_cache_status;
        proxy_hide_header            access-control-allow-origin;
        add_header                   Access-Control-Allow-Origin "*";


        proxy_pass                   http://$backend$request_uri;
        proxy_cache                  my_diskcached;

        proxy_cache_valid            200 1d;
        proxy_cache_valid            403 404 500 501 502 503 2m;
        # END ProxySettings
    }
