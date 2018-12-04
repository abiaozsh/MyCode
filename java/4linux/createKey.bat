rem keytool -genkey -keyalg RSA -keystore clientKeys.key
rem keytool -certreq -keystore clientKeys.key -file clientKeys.csr

rem keytool -import -trustcacerts -keystore clientKeys.key -alias root -file root.csr
keytool -import -keystore clientKeys.key -file clientKeys.csr


rem keytool -genkey -keystore clientKeys.key -keysize 1024 -keypass 123456 -storepass 123456
rem keytool -export -keystore clientKeys.key -file clientKeys.cer
rem keytool -list -v -keystore clientKeys.key
pause