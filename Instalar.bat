powershell -c "Set-ExecutionPolicy RemoteSigned -Scope CurrentUser"

powershell -c "irm get.scoop.sh | iex"

start powershell -c "scoop install gcc; scoop install git; scoop install 7zip; git clone https://github.com/rubeneitor21/Windows-Handler.git; cd window-handler; .\update.ps1; explorer Windows-Handler"