((c-mode . ((dape-configs . ((gdb-test-unit
                              modes (c-mode)
                              command-cwd dape-command-cwd
                              command "gdb"
                              command-args ("--interpreter=dap")
                              compile "make test/unit DEBUG=1"
                              :request "launch"
                              :program "test/unit"))))))
