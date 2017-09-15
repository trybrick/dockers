    location ~* ^/(Get|assets) {
        expires 12h;

        # ProxySettings

        set $backend "hannaford-origin.brickinc.net";

        proxy_set_header             Host "hannaford.gsnrecipes.com";
        proxy_hide_header            access-control-allow-origin;
        add_header                   Access-Control-Allow-Origin "*";
        add_header                   X-Cache $upstream_cache_status;

        proxy_set_header             X-Forwarded-For $remote_addr;

        include                      conf.d/proxy-hide-headers.inc;

        proxy_pass                   http://$backend$request_uri;
        proxy_cache_valid            200 201 12h;
        proxy_cache_valid            403 404 500 501 502 503 5s;
        proxy_cache_lock             on;

        # END ProxySettings
    }
