server {
    listen 80;
    listen 443 ssl;

    server_name hannaford.gsnrecipes.com hannaford.brickinc.net;

    ssl_stapling                     on;
    ssl_stapling_verify              on;

    ssl_certificate                  conf.d/hannaford.gsnrecipes.com.crt;
    ssl_certificate_key              conf.d/hannaford.gsnrecipes.com.key;
    ssl_trusted_certificate          conf.d/hannaford.gsnrecipes.com.crt;

    include                          conf.d/hannaford.gsnrecipes.com.inc;
}

