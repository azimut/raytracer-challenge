((c-mode . ((compile-command . "cd ..; make -B test SANITIZE=1")
            (dape-configs . ((gdb-test-unit
                              modes (c-mode)
                              command-cwd dape-command-cwd
                              command "/usr/local/bin/gdb"
                              command-args ("--interpreter=dap")
                              compile "make -B build/unit DEBUG=1"
                              :request "launch"
                              :program "build/unit"
                              :stopAtBeginningOfMainSubprogram t)
                             (gdb-12cube
                              modes (c-mode)
                              command-cwd dape-command-cwd
                              command "/usr/local/bin/gdb"
                              command-args ("--interpreter=dap")
                              compile "make -B build/12cube DEBUG=1 DIMENSION=100"
                              :request "launch"
                              :program "build/12cube"
                              :stopAtBeginningOfMainSubprogram t))))))
