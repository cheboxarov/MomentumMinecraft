#include "versions.h"
#include "QCoreApplication"
Versions::Versions() {}

QVector<Version> Versions::getLoadedVersions() {
    return loaded_versions;
}

void Versions::loadVersions() {
    Version caves_and_cliffs;
    caves_and_cliffs.name = "CAVES & CLIFFS";
    caves_and_cliffs.description = R"(
новая сборка: 1.16.5 технорпгмагия
и еще 70 модов
4-5 технических (по запросу)
(алярм: никакого индастриала нет,
есть иммерсив инженеринг,форестри,
апплайд энергетикс 2,эвил крафт,
термал фаундейшн и экспеншен)
моды на оптимизацию,
новые мобы, данжи и генерируемые
структуры (как околованиль,
так и остальное) новое оружие,
 новая броня)";
    caves_and_cliffs.name_of_file = "caves_and_cliffs";
    caves_and_cliffs.size = 716052040;
    caves_and_cliffs.id = 0;
    caves_and_cliffs.startcmd = R"(jre\java-runtime-alpha\windows-x64\java-runtime-alpha\bin\javaw.exe -XX:+UnlockExperimentalVMOptions -XX:+UseG1GC -XX:G1NewSizePercent=20 -XX:G1ReservePercent=20 -XX:MaxGCPauseMillis=50 -XX:G1HeapRegionSize=32M -XX:+DisableExplicitGC -XX:+AlwaysPreTouch -XX:+ParallelRefProcEnabled -Xms2048M -Xmx5000M -Dfile.encoding=UTF-8 -XX:HeapDumpPath=MojangTricksIntelDriversForPerformance_javaw.exe_minecraft.exe.heapdump "-Djava.library.path=game\versions\Forge 1.16.5\natives" -Dminecraft.launcher.brand=java-minecraft-launcher -Dminecraft.launcher.version=1.6.84-j -cp "game\libraries\com\turikhay\ca-fixer\1.0\ca-fixer-1.0.jar;game\libraries\net\minecraftforge\forge\1.16.5-36.2.41\forge-1.16.5-36.2.41.jar;game\libraries\org\ow2\asm\asm\9.6\asm-9.6.jar;game\libraries\org\ow2\asm\asm-commons\9.6\asm-commons-9.6.jar;game\libraries\org\ow2\asm\asm-tree\9.6\asm-tree-9.6.jar;game\libraries\org\ow2\asm\asm-util\9.6\asm-util-9.6.jar;game\libraries\org\ow2\asm\asm-analysis\9.6\asm-analysis-9.6.jar;game\libraries\cpw\mods\modlauncher\8.1.3\modlauncher-8.1.3.jar;game\libraries\cpw\mods\grossjava9hacks\1.3.3\grossjava9hacks-1.3.3.jar;game\libraries\net\minecraftforge\accesstransformers\3.0.1\accesstransformers-3.0.1.jar;game\libraries\org\antlr\antlr4-runtime\4.9.1\antlr4-runtime-4.9.1.jar;game\libraries\net\minecraftforge\eventbus\4.0.0\eventbus-4.0.0.jar;game\libraries\net\minecraftforge\forgespi\3.2.0\forgespi-3.2.0.jar;game\libraries\net\minecraftforge\coremods\4.0.6\coremods-4.0.6.jar;game\libraries\net\minecraftforge\unsafe\0.2.0\unsafe-0.2.0.jar;game\libraries\com\electronwill\night-config\core\3.6.3\core-3.6.3.jar;game\libraries\com\electronwill\night-config\toml\3.6.3\toml-3.6.3.jar;game\libraries\org\jline\jline\3.12.1\jline-3.12.1.jar;game\libraries\org\apache\maven\maven-artifact\3.6.3\maven-artifact-3.6.3.jar;game\libraries\net\jodah\typetools\0.8.3\typetools-0.8.3.jar;game\libraries\org\apache\logging\log4j\log4j-api\2.15.0\log4j-api-2.15.0.jar;game\libraries\org\apache\logging\log4j\log4j-core\2.15.0\log4j-core-2.15.0.jar;game\libraries\org\apache\logging\log4j\log4j-slf4j18-impl\2.15.0\log4j-slf4j18-impl-2.15.0.jar;game\libraries\net\minecrell\terminalconsoleappender\1.2.0\terminalconsoleappender-1.2.0.jar;game\libraries\net\sf\jopt-simple\jopt-simple\5.0.4\jopt-simple-5.0.4.jar;game\libraries\org\spongepowered\mixin\0.8.4\mixin-0.8.4.jar;game\libraries\net\minecraftforge\nashorn-core-compat\15.1.1.1\nashorn-core-compat-15.1.1.1.jar;game\libraries\ru\tlauncher\patchy\1.0.0\patchy-1.0.0.jar;game\libraries\oshi-project\oshi-core\1.1\oshi-core-1.1.jar;game\libraries\net\java\dev\jna\jna\4.4.0\jna-4.4.0.jar;game\libraries\net\java\dev\jna\platform\3.4.0\platform-3.4.0.jar;game\libraries\com\ibm\icu\icu4j\66.1\icu4j-66.1.jar;game\libraries\com\mojang\javabridge\1.0.22\javabridge-1.0.22.jar;game\libraries\net\sf\jopt-simple\jopt-simple\5.0.3\jopt-simple-5.0.3.jar;game\libraries\io\netty\netty-all\4.1.25.Final\netty-all-4.1.25.Final.jar;game\libraries\com\google\guava\guava\21.0\guava-21.0.jar;game\libraries\org\apache\commons\commons-lang3\3.5\commons-lang3-3.5.jar;game\libraries\commons-io\commons-io\2.5\commons-io-2.5.jar;game\libraries\commons-codec\commons-codec\1.10\commons-codec-1.10.jar;game\libraries\net\java\jinput\jinput\2.0.5\jinput-2.0.5.jar;game\libraries\net\java\jutils\jutils\1.0.0\jutils-1.0.0.jar;game\libraries\com\mojang\brigadier\1.0.17\brigadier-1.0.17.jar;game\libraries\com\mojang\datafixerupper\4.0.26\datafixerupper-4.0.26.jar;game\libraries\com\google\code\gson\gson\2.8.0\gson-2.8.0.jar;game\libraries\by\ely\authlib\3.11.49.2\authlib-3.11.49.2.jar;game\libraries\org\apache\commons\commons-compress\1.8.1\commons-compress-1.8.1.jar;game\libraries\org\apache\httpcomponents\httpclient\4.3.3\httpclient-4.3.3.jar;game\libraries\commons-logging\commons-logging\1.1.3\commons-logging-1.1.3.jar;game\libraries\org\apache\httpcomponents\httpcore\4.3.2\httpcore-4.3.2.jar;game\libraries\it\unimi\dsi\fastutil\8.2.1\fastutil-8.2.1.jar;game\libraries\org\apache\logging\log4j\log4j-api\2.8.1\log4j-api-2.8.1.jar;game\libraries\org\apache\logging\log4j\log4j-core\2.8.1\log4j-core-2.8.1.jar;game\libraries\org\lwjgl\lwjgl\3.2.2\lwjgl-3.2.2.jar;game\libraries\org\lwjgl\lwjgl-jemalloc\3.2.2\lwjgl-jemalloc-3.2.2.jar;game\libraries\org\lwjgl\lwjgl-openal\3.2.2\lwjgl-openal-3.2.2.jar;game\libraries\org\lwjgl\lwjgl-opengl\3.2.2\lwjgl-opengl-3.2.2.jar;game\libraries\org\lwjgl\lwjgl-glfw\3.2.2\lwjgl-glfw-3.2.2.jar;game\libraries\org\lwjgl\lwjgl-stb\3.2.2\lwjgl-stb-3.2.2.jar;game\libraries\org\lwjgl\lwjgl-tinyfd\3.2.2\lwjgl-tinyfd-3.2.2.jar;game\libraries\com\mojang\text2speech\1.11.3\text2speech-1.11.3.jar;game\versions\Forge 1.16.5\Forge 1.16.5.jar" -XX:+IgnoreUnrecognizedVMOptions --add-exports=java.base/sun.security.util=ALL-UNNAMED --add-exports=jdk.naming.dns/com.sun.jndi.dns=java.naming --add-opens=java.base/java.util.jar=ALL-UNNAMED --add-opens=java.base/java.lang=ALL-UNNAMED -Xss2M cpw.mods.modlauncher.Launcher --username nickname --version "Forge 1.16.5" --gameDir game --assetsDir game\assets --assetIndex 1.16 --uuid 4d182744387f3826afc7efa86783a141 --accessToken 4d182744387f3826afc7efa86783a141 --userType legacy --versionType modified --width 925 --height 530 --launchTarget fmlclient --fml.forgeVersion 36.2.41 --fml.mcVersion 1.16.5 --fml.forgeGroup net.minecraftforge --fml.mcpVersion 20210115.111550)";
    caves_and_cliffs.url_to_download = "https://minecraft_chebox.hb.ru-msk.vkcs.cloud/caves_and_cliffs.zip";
    caves_and_cliffs.minecraft_version = "1.16.5";
    caves_and_cliffs.photo = ":/image/images/btn.png";
    loaded_versions.append(caves_and_cliffs);
    Version test;
    test.name = "TEST NAME";
    test.description = "TEST description";
    test.name_of_file = "test";
    test.minecraft_version = "123";
    test.id = 1;
    test.startcmd = "123";
    test.url_to_download = "123";
    test.size = 123;
    test.photo = ":/image/images/btn1.png";
    loaded_versions.append(test);
}
