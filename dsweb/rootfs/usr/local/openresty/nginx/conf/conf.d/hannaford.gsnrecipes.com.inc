    access_log  /var/log/nginx/access.log  logalt;

    resolver                         8.8.8.8 8.8.4.4;

    proxy_redirect                   off;
    proxy_set_header                 X-Real-IP       $remote_addr;
    proxy_set_header                 X-Forwarded-For $proxy_add_x_forwarded_for;

    proxy_connect_timeout            30s;
    proxy_send_timeout               30s;
    proxy_read_timeout               30s;

    # prevent client headers from going to origin
    proxy_pass_request_headers       off;

    proxy_ignore_headers             Vary Expires Set-Cookie Cache-Control;
    proxy_pass_header                P3P;
    proxy_cache_min_uses             2;
    proxy_ssl_server_name            on;
    proxy_intercept_errors           on;

    # remove some headers
    set_by_lua $genv '
       ngx.header["Server"] = nil
       return "."
    ';

    location / {
        return 301 https://www.hannaford.com$request_uri;
    }

    include                          conf.d/hannaford.proxy.inc;
