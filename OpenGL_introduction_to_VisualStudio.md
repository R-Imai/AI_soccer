# OpenGLのVisualStudioへの導入
openGLをVisualStudioで使用可能にするには、glut.h、glut32.lib、glut32.dllを所定の場所に置く必要があります。<br>
それぞれ以下の場所にファイルを置いてください。<br>
`glut.h >> "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include\GL"`<br>
`glut32.lib >> "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib"`<br>
`glut32.dll >> "C:\Windows\SysWOW64"`<br>
これで動くようになるはずです。
