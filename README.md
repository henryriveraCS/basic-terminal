<h1> basic-terminal </h1>

<b> Terminal written in C that can run on UNIX machines. It's basically a more minimal(*buggy*) version of your default terminal :D </b>

<h2>Dependancies</h2>
<p>Make sure you have the following installed (more might be added as more networking features are added.</p>
<ul>
  <li>curl/curl.h <code>sudo apt-get install libcurl4-openssl-dev</code></li>
</ul>

<h2> HOW TO COMPILE/RUN ON MACOSX OR LINUX</h2>
<ul>
  <li>1. <code>git clone https://github.com/henryriveraCS/basic-terminal</code> </li>
  <li>2. Run <code>make</code> inside the <code>basic-terminal</code> folder</li>
  <li>3. You should have a program called <code>basic-terminal</code> in your directory. Run it by typing <code>./basic-terminal</code></li>
  <li>4. If you need to rebuild it just run <code>make new</code></li>
</ul>

<h3>CURRENT FUNCTIONALITY</h3>
<p>
  Basic Terminal currently has the same functionalities that you would expect from a basic command line terminal such as: 
  <br>
  <code>ls, cd, pwd, touch, curl, cat</code><p>You can check more commands by running <code>help</code> from within the terminal.</p> You can exit with a simple <code>exit</code>
</p>
  <h3>TODO</h3>
  <p>I will be adding a VIM-like file editor and a few more "basic" commands such as <code>ftp, ping, maybe git, etc...</code></p>
  <p>(please refer to TODO.md for more details)</p>
