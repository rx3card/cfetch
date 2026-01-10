#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// Estructura para opciones de línea de comandos
typedef struct Options {
    bool show_help;
    bool show_version;
    bool no_color;
    bool simple_mode;
    bool use_default_logo;
} Options;

#ifdef _WIN32
    #include <windows.h>
    #define PATH_SEPARATOR '\\'
    #define LINE_CLEAR "cls"
#else
    #include <unistd.h>
    #include <sys/utsname.h>
    #include <sys/sysinfo.h>
    #include <sys/statvfs.h>
    #include <pwd.h>
#endif

#define MAX_INFO 512
#define MAX_LINES 40
#define LOGO_WIDTH 50

// Códigos de color ANSI
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define BRED "\033[1;31m"
#define BGREEN "\033[1;32m"
#define BYELLOW "\033[1;33m"
#define BBLUE "\033[1;34m"
#define BMAGENTA "\033[1;35m"
#define BCYAN "\033[1;36m"
#define BWHITE "\033[1;37m"
#define GRAY "\033[1;30m"

typedef enum {
    OS_WINDOWS,
    OS_WINDOWS_SERVER,
    OS_UBUNTU,
    OS_DEBIAN,
    OS_FEDORA,
    OS_ARCH,
    OS_MANJARO,
    OS_OPENSUSE,
    OS_OPENSUSE_LEAP,
    OS_OPENSUSE_TUMBLEWEED,
    OS_CENTOS,
    OS_RHEL,
    OS_MINT,
    OS_ELEMENTARY,
    OS_POP_OS,
    OS_KALI,
    OS_ALPINE,
    OS_GENTOO,
    OS_VOID,
    OS_SLACKWARE,
    OS_SOLUS,
    OS_ANDROID,
    OS_MACOS,
    OS_FREEBSD,
    OS_OPENBSD,
    OS_NETBSD,
    OS_DRAGONFLY,
    OS_LINUX_GENERIC
} OSType;


typedef struct {
    OSType type;
    // Información del sistema
    char os[MAX_INFO];
    char hostname[MAX_INFO];
    char username[MAX_INFO];
    char kernel[MAX_INFO];
    char uptime[MAX_INFO];
    char packages[MAX_INFO];
    char shell[MAX_INFO];
    char resolution[MAX_INFO];
    char de[MAX_INFO];
    char wm[MAX_INFO];
    char wm_theme[MAX_INFO];
    char theme[MAX_INFO];
    char icons[MAX_INFO];
    char terminal[MAX_INFO];
    char terminal_font[MAX_INFO];
    char cpu[MAX_INFO];
    char gpu[MAX_INFO];
    char memory[MAX_INFO];
    char disk[MAX_INFO];
    
    // Logo y colores
    const char **logo;
    int logo_lines;
    
    // Opciones
    Options options;
    
    // Colores dinámicos
    const char *color_label;    // Color para las etiquetas (ej: "OS:", "Host:")
    const char *color_value;    // Color para los valores
    const char *color_bar;      // Color para la barra de progreso
    const char *color_logo;     // Color principal del logo
    const char *color_accent;   // Color de acento para el logo
} SystemInfo;

// Prototipos de funciones
void show_help();
void show_version();
void parse_arguments(int argc, char *argv[], SystemInfo *info);
void print_simple_info(const SystemInfo *info);

// Función para calcular el ancho visual real (sin códigos ANSI)
int get_visual_width(const char *str) {
    int width = 0;
    int in_escape = 0;
    
    for (const char *p = str; *p; p++) {
        if (*p == '\033') {
            in_escape = 1;
        } else if (in_escape) {
            if (*p == 'm') in_escape = 0;
        } else {
            width++;
        }
    }
    return width;
}

// ==================== CONFIGURACIÓN ====================

// ==================== ESTRUCTURA DE LOGOS ====================

typedef struct {
    const char *name;          // Nombre del sistema operativo
    const char *const *logo;   // Array de strings con el logo
    int lines;                 // Número de líneas del logo
    const char *color;         // Color principal del logo
} OSLogo;

// ==================== LOGOS ====================

// Logo personalizado por defecto (se usará si está definido USE_DEFAULT_LOGO)
const char *default_custom_logo[] = {
    BBLUE"                "BGREEN"+-"RESET"                    "RESET,
    BBLUE"         "BGREEN"..."BBLUE"   "BGREEN".@%."BBLUE"   "BGREEN"..."BBLUE"             "RESET,
    BBLUE"      "BGREEN".:@@-"BBLUE"    "BGREEN"*@@+"BBLUE"    "BGREEN"=@@-"BBLUE"           "RESET,
    BBLUE"     "BGREEN":@@*@%."BBLUE"   "BGREEN"+@@="BBLUE"   "BGREEN".%@*@@:"BBLUE"         "RESET,
    BBLUE"    "BGREEN"=@%."BBLUE" ="BGREEN"@@=."BBLUE" ."BGREEN"@%."BBLUE" .="BGREEN"@@="BBLUE" ."BGREEN"@@-"BBLUE"        "RESET,
    BBLUE"   "BGREEN".@@."BBLUE"    +"BGREEN"@@@@@@@@@="BBLUE"     .         "RESET,
    BBLUE"   "BGREEN":@%"BBLUE"       "BGREEN".*@@@%."BBLUE"    "BGREEN"......."BBLUE"       "RESET,
    BBLUE"   "BGREEN".@@."BBLUE"    -"BGREEN"@@@@@@@@@-"BBLUE"   "BGREEN".+@@."BBLUE"        "RESET,
    BBLUE"    "BGREEN":@@: .@@+."BBLUE" "BGREEN".@%."BBLUE" "BGREEN".+@@: :@@."BBLUE"         "RESET,
    BBLUE"     "BGREEN".%@@@@."BBLUE"   "BGREEN".@%."BBLUE"   "BGREEN":@@@@%"BBLUE"          "RESET,
    BBLUE"       "BGREEN".*@%."BBLUE"   "BGREEN".@@."BBLUE"   "BGREEN".@%*."BBLUE"           "RESET,
    BBLUE"          ..   "BGREEN".@@."BBLUE"   ..              "RESET,
    BBLUE"               "BGREEN".@@."BBLUE"                   "RESET,
    BBLUE"               "BGREEN".@@:"BBLUE"                   "RESET,
    BBLUE"                "BGREEN"@@-"BBLUE"                   "RESET,
    BBLUE"                "BGREEN":++"BBLUE"                   "RESET
};

// ==================== LOGOS DE SISTEMAS OPERATIVOS ====================

// Windows
const char *windows_logo[] = {
    "        "BRED",.=:!!t3Z3z.,"RESET,
    "       "BRED":tt:::tt333EE3"RESET,
    "       "BRED"Et:::ztt33"RESET"EEEL "BGREEN"@Ee.,      ..,"RESET,
    "      "BRED";tt:::tt333"RESET"EE7 "BGREEN";EEEEEEttttt33#"RESET,
    "     "BRED":Et:::zt333"RESET"EEQ. "BGREEN"$EEEEEttttt33QL"RESET,
    "     "BRED"it::::tt333"RESET"EEF "BGREEN"@EEEEEEttttt33F"RESET,
    "    "BRED";3=*^```\"*4"RESET"EEV "BGREEN":EEEEEEttttt33@."RESET,
    "    "BCYAN",.=::::!t=., "BRED"`"RESET" "BGREEN"@EEEEEEtttz33QF"RESET,
    "   "BCYAN";::::::::zt33)"RESET":Z3z..  "BGREEN"``"RESET" ,..g.",
    "  "BCYAN":t::::::::tt33."RESET":Z3z..  "BGREEN"``"RESET" ,..g.",
    "  "BCYAN"i::::::::zt33"RESET"F AEEEtttt::::ztF",
    " "BCYAN";:::::::::t33"RESET"V ;EEEttttt::::t3",
    " "BCYAN"E::::::::zt33"RESET"L @EEEtttt::::z3F",
    ""BCYAN"{3=*^```\"*4E3)"RESET" ;EEEtttt:::::tZ`",
    "             ` :EEEEtttt::::z7",
    "                 \"VEzjt:;;z>*`"
};

// Ubuntu
const char *ubuntu_logo[] = {
    "            "BRED".-/+oossssoo+/-."RESET,
    "        "BRED"`:+ssssssssssssssssss+:`"RESET,
    "      "BRED"-+sssssssssssssssssssyyssss+-"RESET,
    "    "BRED".ossssssssssssssssss"RESET"dMMMNy"BRED"sssso."RESET,
    "   "BRED"/sssssssssss"RESET"hdmmNNmmyNMMMMh"BRED"ssssss/"RESET,
    "  "BRED"+sssssssss"RESET"hm"BRED"yd"RESET"MMMMMMMNddddy"BRED"ssssssss+"RESET,
    " "BRED"/ssssssss"RESET"hNMMM"BRED"yh"RESET"hyyyyhmNMMMNh"BRED"ssssssss/"RESET,
    ""BRED".ssssssss"RESET"dMMMNh"BRED"ssssssssss"RESET"hNMMMd"BRED"ssssssss."RESET,
    ""BRED"+ssss"RESET"hhhyNMMNy"BRED"ssssssssssss"RESET"yNMMMy"BRED"sssssss+"RESET,
    ""BRED"oss"RESET"yNMMMNyMMh"BRED"ssssssssssssss"RESET"hmmmh"BRED"ssssssso"RESET,
    ""BRED"oss"RESET"yNMMMNyMMh"BRED"sssssssssssssshmmmhssssssso"RESET,
    ""BRED"+ssss"RESET"hhhyNMMNy"BRED"ssssssssssss"RESET"yNMMMy"BRED"sssssss+"RESET,
    ""BRED".ssssssss"RESET"dMMMNh"BRED"ssssssssss"RESET"hNMMMd"BRED"ssssssss."RESET,
    " "BRED"/ssssssss"RESET"hNMMM"BRED"yh"RESET"hyyyyhdNMMMNh"BRED"ssssssss/"RESET,
    "  "BRED"+sssssssss"RESET"dm"BRED"yd"RESET"MMMMMMMMddddy"BRED"ssssssss+"RESET,
    "   "BRED"/sssssssssss"RESET"hdmNNNNmyNMMMMh"BRED"ssssss/"RESET,
    "    "BRED".ossssssssssssssssss"RESET"dMMMNy"BRED"sssso."RESET,
    "      "BRED"-+sssssssssssssssss"RESET"yyy"BRED"ssss+-"RESET,
    "        "BRED"`:+ssssssssssssssssss+:`"RESET,
    "            "BRED".-/+oossssoo+/-."RESET
};

// Arch Linux
const char *arch_logo[] = {
    "                   "BCYAN"-`"RESET,
    "                  "BCYAN".o+`"RESET,
    "                 "BCYAN"`ooo/"RESET,
    "                "BCYAN"`+oooo:"RESET,
    "               "BCYAN"`+oooooo:"RESET,
    "               "BCYAN"-+oooooo+:"RESET,
    "             "BCYAN"`/:-:++oooo+:"RESET,
    "            "BCYAN"`/++++/+++++++:"RESET,
    "           "BCYAN"`/++++++++++++++:"RESET,
    "          "BCYAN"`/+++o"RESET"oooooooo"BCYAN"oooo/`"RESET,
    "         "RESET"./ooosssso++osssssso"BCYAN"+`"RESET,
    "        "RESET".oossssso-````/ossssss+"BCYAN"`"RESET,
    "       "RESET"-osssssso.      :ssssssso."RESET,
    "      "RESET":osssssss/        osssso+++."RESET,
    "     "RESET"/ossssssss/        +ssssooo/-"RESET,
    "   "RESET"`/ossssso+/:-        -:/+osssso+-"RESET,
    "  "RESET"`+sso+:-`                 `.-/+oso:"RESET,
    " "RESET"`++:.                           `-/+/"RESET,
    " "RESET".`                                 `/"RESET,
};

// Linux
const char *linux_logo[] = {
    "   "BWHITE"    .--.     "RESET"   ",
    "  "BWHITE"   |o_o |    "RESET"   ",
    "  "BWHITE"   |:_/ |    "RESET"   ",
    "  "BWHITE"  //    \\\\   "RESET"  ",
    " "BWHITE" (|     | )  "RESET"   ",
    " "BWHITE"/'\\\\_   _/`\\\\  "RESET"  ",
    " "BWHITE"\\\\  "RESET" "BWHITE"\\\\___/  "RESET" ",
    "  "BWHITE"\\\\_/\\\\_/   "RESET"     ",
    "    "BWHITE"||  |   "RESET"     ",
    "    "BWHITE"\\\\  |   "RESET"     ",
    "     "BWHITE"\\\\_/    "RESET"    ",
    "    "BWHITE"  |\\\\_/|  "RESET"   ",
    "    "BWHITE"  \\\\_ _/   "RESET"  ",
    "    "BWHITE"   (oo)\\\\___/  "RESET"",
    "   "BWHITE"   /  \\\\       "RESET"",
    "  "BWHITE"  /    ||----w |"RESET"",
    "  "BWHITE"  \\\\    ||     ||"RESET"",
    "   "BWHITE"   \\\\   ||    ||"RESET"",
    "    "BWHITE"   \\\\  ||   || "RESET"",
    "     "BWHITE"   \\\\_||___||_/"RESET"",
    "      "BWHITE"   (__)  (__)"RESET""
};

// ==================== FUNCIONES DE DETECCIÓN ====================

#ifdef _WIN32
void enable_ansi_colors() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(h, &mode);
    SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

OSType detect_os() {
    OSVERSIONINFOEX osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    
    if (GetVersionEx((OSVERSIONINFO*)&osvi)) {
        if (osvi.wProductType == VER_NT_DOMAIN_CONTROLLER || 
            osvi.wProductType == VER_NT_SERVER) {
            return OS_WINDOWS_SERVER;
        }
    }
    return OS_WINDOWS;
}

void get_os_info(SystemInfo *info) {
    OSVERSIONINFOEX osvi;
    ZeroMemory(&osvi, sizeof(osvi));
    osvi.dwOSVersionInfoSize = sizeof(osvi);
    
    HMODULE h = GetModuleHandle(TEXT("ntdll.dll"));
    if (h) {
        typedef LONG (WINAPI* RtlGetVersion)(PRTL_OSVERSIONINFOW);
        RtlGetVersion fn = (RtlGetVersion)GetProcAddress(h, "RtlGetVersion");
        if (fn) fn((PRTL_OSVERSIONINFOW)&osvi);
    }
    
    char *ed = "Unknown";
    if (osvi.dwMajorVersion == 10 && osvi.dwBuildNumber >= 22000) ed = "Windows 11";
    else if (osvi.dwMajorVersion == 10) ed = "Windows 10";
    else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 3) ed = "Windows 8.1";
    else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2) ed = "Windows 8";
    else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1) ed = "Windows 7";
    
    sprintf(info->os, "%s (Build %lu)", ed, osvi.dwBuildNumber);
    sprintf(info->kernel, "%lu.%lu.%lu", osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber);
}

void get_hostname(SystemInfo *info) {
    DWORD s = MAX_INFO;
    GetComputerNameA(info->hostname, &s);
}

void get_username(SystemInfo *info) {
    DWORD s = MAX_INFO;
    GetUserNameA(info->username, &s);
}

void get_uptime(SystemInfo *info) {
    DWORD t = GetTickCount64() / 1000;
    int d = t / 86400, h = (t % 86400) / 3600, m = (t % 3600) / 60;
    if (d > 0) sprintf(info->uptime, "%dd %dh %dm", d, h, m);
    else if (h > 0) sprintf(info->uptime, "%dh %dm", h, m);
    else sprintf(info->uptime, "%dm", m);
}

void get_packages(SystemInfo *info) {
    FILE *f = popen("scoop list 2>nul | find /c /v \"\"", "r");
    int count = 0;
    if (f) {
        fscanf(f, "%d", &count);
        pclose(f);
        if (count > 0) sprintf(info->packages, "%d (scoop)", count);
        else strcpy(info->packages, "");
    } else strcpy(info->packages, "");
}

void get_shell(SystemInfo *info) {
    char *c = getenv("COMSPEC");
    if (c) {
        char *s = strrchr(c, '\\');
        strcpy(info->shell, s ? s + 1 : c);
    } else strcpy(info->shell, "cmd.exe");
}

void get_resolution(SystemInfo *info) {
    sprintf(info->resolution, "%dx%d", GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

void get_de(SystemInfo *info) {
    strcpy(info->de, "Aero");
}

void get_wm(SystemInfo *info) {
    strcpy(info->wm, "Explorer");
}

void get_theme(SystemInfo *info) {
    HKEY k;
    DWORD data, size = sizeof(data);
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                      0, KEY_READ, &k) == ERROR_SUCCESS) {
        if (RegQueryValueExA(k, "AppsUseLightTheme", NULL, NULL, (LPBYTE)&data, &size) == ERROR_SUCCESS) {
            strcpy(info->theme, data == 0 ? "Dark" : "Light");
        }
        RegCloseKey(k);
    }
    if (!strlen(info->theme)) strcpy(info->theme, "");
}

void get_icons(SystemInfo *info) {
    strcpy(info->icons, "Windows 11");
}

void get_terminal(SystemInfo *info) {
    char *term = getenv("WT_SESSION");
    if (term) strcpy(info->terminal, "Windows Terminal");
    else strcpy(info->terminal, "cmd.exe");
}

void get_cpu(SystemInfo *info) {
    HKEY k;
    char d[MAX_INFO];
    DWORD s = sizeof(d);
    
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
                      0, KEY_READ, &k) == ERROR_SUCCESS) {
        if (RegQueryValueExA(k, "ProcessorNameString", NULL, NULL, (LPBYTE)d, &s) == ERROR_SUCCESS) {
            char *src = d, *dst = d;
            int sp = 0;
            while (*src) {
                if (*src == ' ') {
                    if (!sp) { *dst++ = *src; sp = 1; }
                } else { *dst++ = *src; sp = 0; }
                src++;
            }
            *dst = '\0';
            strcpy(info->cpu, d);
        }
        RegCloseKey(k);
    }
    if (!strlen(info->cpu)) strcpy(info->cpu, "Unknown");
}

void get_gpu(SystemInfo *info) {
    DISPLAY_DEVICE dd;
    ZeroMemory(&dd, sizeof(dd));
    dd.cb = sizeof(dd);
    strcpy(info->gpu, EnumDisplayDevices(NULL, 0, &dd, 0) ? dd.DeviceString : "Unknown");
}

void get_memory(SystemInfo *info) {
    MEMORYSTATUSEX m;
    m.dwLength = sizeof(m);
    GlobalMemoryStatusEx(&m);
    sprintf(info->memory, "%llu MiB / %llu MiB",
            (m.ullTotalPhys - m.ullAvailPhys) / 1048576,
            m.ullTotalPhys / 1048576);
}

void get_disk(SystemInfo *info) {
    ULARGE_INTEGER f, t, tf;
    if (GetDiskFreeSpaceExA("C:\\", &f, &t, &tf)) {
        sprintf(info->disk, "%llu GiB / %llu GiB",
                (t.QuadPart - tf.QuadPart) / 1073741824,
                t.QuadPart / 1073741824);
    } else strcpy(info->disk, "Unknown");
}

#else

void enable_ansi_colors() {
    // En sistemas Unix-like, los colores ANSI generalmente están habilitados por defecto
}

OSType detect_os() {
    struct utsname unameData;
    if (uname(&unameData) != 0) {
        return OS_LINUX_GENERIC;
    }

    // Detección de macOS
    if (strstr(unameData.sysname, "Darwin")) {
        return OS_MACOS;
    }

    // Detección de BSDs
    if (strstr(unameData.sysname, "FreeBSD")) return OS_FREEBSD;
    if (strstr(unameData.sysname, "OpenBSD")) return OS_OPENBSD;
    if (strstr(unameData.sysname, "NetBSD")) return OS_NETBSD;
    if (strstr(unameData.sysname, "DragonFly")) return OS_DRAGONFLY;

    // Para Linux, intentamos leer /etc/os-release
    FILE *fp = fopen("/etc/os-release", "r");
    if (fp) {
        char line[256];
        char id[50] = "";
        
        while (fgets(line, sizeof(line), fp)) {
            // Buscar la línea que comienza con ID=
            if (strncmp(line, "ID=", 3) == 0) {
                strncpy(id, line + 3, sizeof(id) - 1);
                // Eliminar comillas y saltos de línea
                char *q = strchr(id, '"');
                if (q) {
                    memmove(id, q + 1, strlen(q));
                    q = strchr(id, '"');
                    if (q) *q = '\0';
                } else {
                    // Si no hay comillas, eliminar el salto de línea
                    q = strchr(id, '\n');
                    if (q) *q = '\0';
                }
                break;
            }
        }
        fclose(fp);
        
        // Mapear IDs a los tipos de OS
        if (strcmp(id, "ubuntu") == 0) return OS_UBUNTU;
        if (strcmp(id, "debian") == 0) return OS_DEBIAN;
        if (strcmp(id, "fedora") == 0) return OS_FEDORA;
        if (strcmp(id, "arch") == 0 || strcmp(id, "archlinux") == 0) return OS_ARCH;
        if (strcmp(id, "manjaro") == 0) return OS_MANJARO;
        if (strcmp(id, "opensuse-tumbleweed") == 0) return OS_OPENSUSE_TUMBLEWEED;
        if (strcmp(id, "opensuse-leap") == 0) return OS_OPENSUSE_LEAP;
        if (strcmp(id, "opensuse") == 0) return OS_OPENSUSE;
        if (strcmp(id, "centos") == 0) return OS_CENTOS;
        if (strcmp(id, "rhel") == 0) return OS_RHEL;
        if (strcmp(id, "linuxmint") == 0) return OS_MINT;
        if (strcmp(id, "elementary") == 0) return OS_ELEMENTARY;
        if (strcmp(id, "pop") == 0) return OS_POP_OS;
        if (strcmp(id, "kali") == 0) return OS_KALI;
        if (strcmp(id, "alpine") == 0) return OS_ALPINE;
        if (strcmp(id, "gentoo") == 0) return OS_GENTOO;
        if (strcmp(id, "void") == 0) return OS_VOID;
        if (strcmp(id, "slackware") == 0) return OS_SLACKWARE;
        if (strcmp(id, "solus") == 0) return OS_SOLUS;
        if (strcmp(id, "android") == 0) return OS_ANDROID;
    }
    
    // Si no se pudo determinar, devolver el tipo genérico
    return OS_LINUX_GENERIC;
}

void get_os_info(SystemInfo *info) {
    FILE *f = fopen("/etc/os-release", "r");
    if (f) {
        char line[256];
        while (fgets(line, sizeof(line), f)) {
            if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
                char *start = strchr(line, '"');
                if (start) {
                    start++;
                    char *end = strrchr(start, '"');
                    if (end) {
                        *end = '\0';
                        strcpy(info->os, start);
                    }
                }
                break;
            }
        }
        fclose(f);
    }
    if (!strlen(info->os)) strcpy(info->os, "Linux");
}

void get_hostname(SystemInfo *info) {
    gethostname(info->hostname, MAX_INFO);
}

void get_username(SystemInfo *info) {
    struct passwd *pw = getpwuid(getuid());
    strcpy(info->username, pw ? pw->pw_name : "unknown");
}

void get_uptime(SystemInfo *info) {
    struct sysinfo si;
    if (sysinfo(&si) == 0) {
        int d = si.uptime / 86400, h = (si.uptime % 86400) / 3600, m = (si.uptime % 3600) / 60;
        if (d > 0) sprintf(info->uptime, "%dd %dh %dm", d, h, m);
        else if (h > 0) sprintf(info->uptime, "%dh %dm", h, m);
        else sprintf(info->uptime, "%dm", m);
    } else strcpy(info->uptime, "");
}

void get_packages(SystemInfo *info) {
    FILE *f = NULL;
    int count = 0;
    
    if ((f = popen("dpkg -l 2>/dev/null | grep '^ii' | wc -l", "r"))) {
        fscanf(f, "%d", &count);
        pclose(f);
        if (count > 0) { sprintf(info->packages, "%d (dpkg)", count); return; }
    }
    if ((f = popen("pacman -Q 2>/dev/null | wc -l", "r"))) {
        fscanf(f, "%d", &count);
        pclose(f);
        if (count > 0) { sprintf(info->packages, "%d (pacman)", count); return; }
    }
    strcpy(info->packages, "");
}

void get_shell(SystemInfo *info) {
    char *sh = getenv("SHELL");
    if (sh) {
        char *s = strrchr(sh, '/');
        strcpy(info->shell, s ? s + 1 : sh);
    } else strcpy(info->shell, "sh");
}

void get_resolution(SystemInfo *info) {
    FILE *f = popen("xdpyinfo 2>/dev/null | grep dimensions", "r");
    if (f) {
        char buf[256];
        if (fgets(buf, sizeof(buf), f)) {
            int w, h;
            if (sscanf(buf, " dimensions: %dx%d", &w, &h) == 2)
                sprintf(info->resolution, "%dx%d", w, h);
        }
        pclose(f);
    }
    if (!strlen(info->resolution)) strcpy(info->resolution, "");
}

void get_de(SystemInfo *info) {
    char *de = getenv("XDG_CURRENT_DESKTOP");
    if (!de) de = getenv("DESKTOP_SESSION");
    strcpy(info->de, de ? de : "");
}

void get_wm(SystemInfo *info) {
    char *wm = getenv("XDG_CURRENT_DESKTOP");
    if (!wm) wm = getenv("DESKTOP_SESSION");
    if (wm) {
        if (strstr(wm, "GNOME")) strcpy(info->wm, "Mutter");
        else if (strstr(wm, "KDE")) strcpy(info->wm, "KWin");
        else if (strstr(wm, "XFCE")) strcpy(info->wm, "Xfwm4");
        else strcpy(info->wm, wm);
    } else strcpy(info->wm, "");
}

void get_theme(SystemInfo *info) {
    FILE *f = popen("gsettings get org.gnome.desktop.interface gtk-theme 2>/dev/null", "r");
    if (f) {
        char buf[MAX_INFO];
        if (fgets(buf, sizeof(buf), f)) {
            char *start = strchr(buf, '\'');
            if (start) {
                start++;
                char *end = strrchr(start, '\'');
                if (end) {
                    *end = '\0';
                    strcpy(info->theme, start);
                }
            }
        }
        pclose(f);
    }
    if (!strlen(info->theme)) strcpy(info->theme, "");
}

void get_icons(SystemInfo *info) {
    FILE *f = popen("gsettings get org.gnome.desktop.interface icon-theme 2>/dev/null", "r");
    if (f) {
        char buf[MAX_INFO];
        if (fgets(buf, sizeof(buf), f)) {
            char *start = strchr(buf, '\'');
            if (start) {
                start++;
                char *end = strrchr(start, '\'');
                if (end) {
                    *end = '\0';
                    strcpy(info->icons, start);
                }
            }
        }
        pclose(f);
    }
    if (!strlen(info->icons)) strcpy(info->icons, "");
}

void get_terminal(SystemInfo *info) {
    char *term = getenv("TERM_PROGRAM");
    if (!term) term = getenv("TERM");
    strcpy(info->terminal, term ? term : "");
}

void get_cpu(SystemInfo *info) {
    FILE *f = fopen("/proc/cpuinfo", "r");
    if (f) {
        char line[256];
        while (fgets(line, sizeof(line), f)) {
            if (strncmp(line, "model name", 10) == 0) {
                char *colon = strchr(line, ':');
                if (colon) {
                    colon += 2;
                    char *nl = strchr(colon, '\n');
                    if (nl) *nl = '\0';
                    strcpy(info->cpu, colon);
                    break;
                }
            }
        }
        fclose(f);
    }
    if (!strlen(info->cpu)) strcpy(info->cpu, "Unknown");
}

void get_gpu(SystemInfo *info) {
    FILE *f = popen("lspci 2>/dev/null | grep -i vga", "r");
    if (f) {
        char buf[MAX_INFO];
        if (fgets(buf, sizeof(buf), f)) {
            char *colon = strchr(buf, ':');
            if (colon) {
                colon += 2;
                char *nl = strchr(colon, '\n');
                if (nl) *nl = '\0';
                strcpy(info->gpu, colon);
            }
        }
        pclose(f);
    }
    if (!strlen(info->gpu)) strcpy(info->gpu, "Unknown");
}

void get_memory(SystemInfo *info) {
    struct sysinfo si;
    if (sysinfo(&si) == 0) {
        unsigned long total = si.totalram / 1048576;
        unsigned long used = (si.totalram - si.freeram) / 1048576;
        sprintf(info->memory, "%lu MiB / %lu MiB", used, total);
    } else strcpy(info->memory, "Unknown");
}

void get_disk(SystemInfo *info) {
    struct statvfs stat;
    if (statvfs("/", &stat) == 0) {
        unsigned long total = (stat.f_blocks * stat.f_frsize) / 1073741824;
        unsigned long used = ((stat.f_blocks - stat.f_bfree) * stat.f_frsize) / 1073741824;
        sprintf(info->disk, "%lu GiB / %lu GiB", used, total);
    } else strcpy(info->disk, "Unknown");
}

void get_kernel(SystemInfo *info) {
    struct utsname u;
    if (uname(&u) == 0) strcpy(info->kernel, u.release);
    else strcpy(info->kernel, "");
}

#endif

// ==================== FUNCIÓN DE DETECCIÓN DE COLORES DINÁMICOS ====================

void detect_os_info(SystemInfo *info) {
    OSType type = detect_os();
    info->type = type;
    
    // Solo establecer los colores si no se está usando el logo por defecto
    if (!info->options.use_default_logo) {
        switch (type) {
            case OS_WINDOWS:
                info->color_label = BWHITE;
                info->color_value = BYELLOW;
                info->color_bar = BRED;
                info->color_logo = BCYAN;
                info->color_accent = BMAGENTA;
                break;
            case OS_UBUNTU:
                info->color_label = BWHITE;
                info->color_value = BYELLOW;
                info->color_bar = BRED;
                info->color_logo = BRED;
                info->color_accent = BGREEN;
                break;
            case OS_ARCH:
                info->color_label = BWHITE;
                info->color_value = BYELLOW;
                info->color_bar = BRED;
                info->color_logo = BCYAN;
                info->color_accent = BMAGENTA;
                break;
            default:
                info->color_label = BWHITE;
                info->color_value = BYELLOW;
                info->color_bar = BRED;
                info->color_logo = BYELLOW;
                info->color_accent = BGREEN;
                break;
        }
    } else {
        // Configuración para el logo por defecto
        info->color_label = BWHITE;
        info->color_value = BYELLOW;
        info->color_bar = BRED;
        info->color_accent = BMAGENTA;
    }
}

void set_os_colors(SystemInfo *info) {
    // No hacer nada si se está usando el logo por defecto
    if (info->options.use_default_logo) {
        return;
    }
    
    OSType type = info->type;
    const char *original_logo_color = info->color_logo;  // Guardar el color del logo si ya está establecido
    
    switch (type) {
        case OS_WINDOWS:
            info->color_label = BWHITE;
            info->color_value = BYELLOW;
            info->color_bar = BRED;
            info->color_accent = BMAGENTA;
            if (!original_logo_color) info->color_logo = BCYAN;  // Solo establecer si no está ya establecido
            break;
        case OS_UBUNTU:
            info->color_label = BWHITE;
            info->color_value = BYELLOW;
            info->color_bar = BRED;
            info->color_accent = BGREEN;
            if (!original_logo_color) info->color_logo = BRED;  // Solo establecer si no está ya establecido
            break;
        case OS_ARCH:
            info->color_label = BWHITE;
            info->color_value = BYELLOW;
            info->color_bar = BRED;
            info->color_accent = BMAGENTA;
            if (!original_logo_color) info->color_logo = BCYAN;  // Solo establecer si no está ya establecido
            break;
        default:
            info->color_label = BWHITE;
            info->color_value = BYELLOW;
            info->color_bar = BRED;
            info->color_accent = BGREEN;
            if (!original_logo_color) info->color_logo = BYELLOW;  // Solo establecer si no está ya establecido
            break;
    }
}




void show_help() {
    printf("CFetch - Una herramienta de información del sistema\n\n");
    printf("Uso: cfetch [OPCIONES]\n\n");
    printf("Opciones:\n");
    printf("  -h, --help        Muestra esta ayuda y sale\n");
    printf("  -v, --version     Muestra la versión del programa\n");
    printf("  --no-color       Desactiva los colores en la salida\n");
    printf("  --simple         Muestra solo la información básica\n");
    printf("  --default-logo   Usa el logo predeterminado en lugar del específico del sistema\n\n");
    printf("Sin opciones, muestra la información completa del sistema con colores.\n");
}

void show_version() {
    printf("CFetch v1.0.0\n");
}

void parse_arguments(int argc, char *argv[], SystemInfo *info) {
    // Valores por defecto
    info->options.show_help = false;
    info->options.show_version = false;
    info->options.no_color = false;
    info->options.simple_mode = false;
    info->options.use_default_logo = false;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            info->options.show_help = true;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            info->options.show_version = true;
        } else if (strcmp(argv[i], "--no-color") == 0) {
            info->options.no_color = true;
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--simple") == 0) {
            info->options.simple_mode = true;
        } else if (strcmp(argv[i], "--default-logo") == 0) {
            info->options.use_default_logo = true;
        }
    }
}

void set_logo(SystemInfo *info) {
    // Forzar el uso del logo personalizado si se especificó --default-logo
    if (info->options.use_default_logo) {
        info->logo = default_custom_logo;
        info->logo_lines = sizeof(default_custom_logo) / sizeof(default_custom_logo[0]);
        info->color_logo = BMAGENTA;
        info->color_label = BWHITE;
        info->color_value = BYELLOW;
        info->color_bar = BRED;
        info->color_accent = BMAGENTA;
        return;
    }
    
    // Si no se especificó --default-logo, usar el logo del sistema operativo
    switch (info->type) {
        case OS_WINDOWS:
            info->logo = windows_logo;
            info->logo_lines = sizeof(windows_logo) / sizeof(windows_logo[0]);
            info->color_logo = BCYAN;
            break;
        case OS_UBUNTU:
            info->logo = ubuntu_logo;
            info->logo_lines = sizeof(ubuntu_logo) / sizeof(ubuntu_logo[0]);
            info->color_logo = BRED;
            break;
        case OS_ARCH:
            info->logo = arch_logo;
            info->logo_lines = sizeof(arch_logo) / sizeof(arch_logo[0]);
            info->color_logo = BCYAN;
            break;
        default:
            info->logo = linux_logo;
            info->logo_lines = sizeof(linux_logo) / sizeof(linux_logo[0]);
            info->color_logo = BYELLOW;
            break;
    }
}

void gather_info(SystemInfo *info) {
    // Guardar las opciones antes de borrar la estructura
    Options saved_options = info->options;
    
    // Limpiar la estructura pero preservar las opciones
    memset(info, 0, sizeof(SystemInfo));
    info->options = saved_options;
    
    // Primero, detectar el sistema operativo sin establecer colores
    info->type = detect_os();
    
    // Obtener la información del sistema operativo
    get_os_info(info);
    get_hostname(info);
    get_username(info);
    get_uptime(info);
    get_packages(info);
    get_shell(info);
    get_resolution(info);
    get_cpu(info);
    get_gpu(info);
    get_memory(info);
    get_disk(info);
    get_de(info);
    get_wm(info);
    get_theme(info);
    get_icons(info);
    get_terminal(info);
    
#ifndef _WIN32
    get_kernel(info);
#endif
    
    // Establecer el logo basado en la opción --default-logo
    set_logo(info);
    
    // Establecer los colores del sistema operativo si no se está usando el logo personalizado
    if (!info->options.use_default_logo) {
        set_os_colors(info);
    }
}

void print_simple_info(const SystemInfo *info) {
    // Versión simple sin logo ni formato especial
    printf("%s@%s\n", info->username, info->hostname);
    printf("OS: %s\n", info->os);
    printf("Kernel: %s\n", info->kernel);
    printf("Uptime: %s\n", info->uptime);
    printf("CPU: %s\n", info->cpu);
    if (strlen(info->gpu) > 0) printf("GPU: %s\n", info->gpu);
    printf("Memory: %s\n", info->memory);
    if (strlen(info->disk) > 0) printf("Disk: %s\n", info->disk);
}

void print_info(SystemInfo *info) {
    // Si está en modo simple, usar la versión simple
    if (info->options.simple_mode) {
        print_simple_info(info);
        return;
    }

    char lines[MAX_LINES][MAX_INFO * 2];
    int idx = 0;
    
    // Header con usuario@hostname
    sprintf(lines[idx++], "%s%s%s@%s%s", info->color_label, info->username, RESET, info->color_label, info->hostname);
    
    // Línea separadora
    char sep[100] = "";
    int len = strlen(info->username) + strlen(info->hostname) + 1;
    for (int i = 0; i < len && i < 98; i++) strcat(sep, "-");
    sprintf(lines[idx++], "%s", sep);
    
    // Información del sistema
    if (strlen(info->os) > 0)
        sprintf(lines[idx++], "%sOS:%s %s", info->color_label, RESET, info->os);
    
    if (strlen(info->hostname) > 0)
        sprintf(lines[idx++], "%sHost:%s %s", info->color_label, RESET, info->hostname);
    
    if (strlen(info->kernel) > 0)
        sprintf(lines[idx++], "%sKernel:%s %s", info->color_label, RESET, info->kernel);
    
    if (strlen(info->uptime) > 0)
        sprintf(lines[idx++], "%sUptime:%s %s", info->color_label, RESET, info->uptime);
    
    if (strlen(info->packages) > 0)
        sprintf(lines[idx++], "%sPackages:%s %s", info->color_label, RESET, info->packages);
    
    if (strlen(info->shell) > 0)
        sprintf(lines[idx++], "%sShell:%s %s", info->color_label, RESET, info->shell);
    
    if (strlen(info->resolution) > 0)
        sprintf(lines[idx++], "%sResolution:%s %s", info->color_label, RESET, info->resolution);
    
    if (strlen(info->de) > 0)
        sprintf(lines[idx++], "%sDE:%s %s", info->color_label, RESET, info->de);
    
    if (strlen(info->wm) > 0)
        sprintf(lines[idx++], "%sWM:%s %s", info->color_label, RESET, info->wm);
    
    if (strlen(info->theme) > 0)
        sprintf(lines[idx++], "%sTheme:%s %s", info->color_label, RESET, info->theme);
    
    if (strlen(info->icons) > 0)
        sprintf(lines[idx++], "%sIcons:%s %s", info->color_label, RESET, info->icons);
    
    if (strlen(info->terminal) > 0)
        sprintf(lines[idx++], "%sTerminal:%s %s", info->color_label, RESET, info->terminal);
    
    if (strlen(info->cpu) > 0)
        sprintf(lines[idx++], "%sCPU:%s %s", info->color_label, RESET, info->cpu);
    
    if (strlen(info->gpu) > 0)
        sprintf(lines[idx++], "%sGPU:%s %s", info->color_label, RESET, info->gpu);
    
    if (strlen(info->memory) > 0)
        sprintf(lines[idx++], "%sMemory:%s %s", info->color_label, RESET, info->memory);
    
    if (strlen(info->disk) > 0)
        sprintf(lines[idx++], "%sDisk:%s %s", info->color_label, RESET, info->disk);
    
    // Línea vacía antes de la barra de colores
    strcpy(lines[idx++], "");
    
    // Barra de colores
    sprintf(lines[idx++], "%s███%s███%s███%s███%s███%s███%s███%s███%s",
            BRED, BGREEN, BYELLOW, BBLUE, BMAGENTA, BCYAN, BWHITE, GRAY, RESET);
    
    // Calcular el ancho máximo del logo
    int max_logo_width = 0;
    for (int i = 0; i < info->logo_lines; i++) {
        int width = get_visual_width(info->logo[i]);
        if (width > max_logo_width) max_logo_width = width;
    }
    
    // Asegurarse de que el logo tenga al menos LOGO_WIDTH de ancho
    if (max_logo_width < LOGO_WIDTH) max_logo_width = LOGO_WIDTH;
    
    printf("\n");
    
    // Calcular offsets para centrar verticalmente
    int total_lines = (info->logo_lines > idx) ? info->logo_lines : idx;
    int logo_offset = (total_lines - info->logo_lines) / 2;
    int info_offset = (total_lines - idx) / 2;
    
    for (int i = 0; i < total_lines; i++) {
        // Calcular índices centrados
        int logo_idx = i - logo_offset;
        int info_idx = i - info_offset;
        
        // Imprimir logo (centrado verticalmente)
        if (logo_idx >= 0 && logo_idx < info->logo_lines) {
            printf("%s", info->logo[logo_idx]);
            // Calcular espacios necesarios para alinear
            int logo_width = get_visual_width(info->logo[logo_idx]);
            int spaces_needed = max_logo_width - logo_width + 3;
            for (int j = 0; j < spaces_needed; j++) printf(" ");
        } else {
            // Si no hay logo en esta línea, imprimir espacios
            for (int j = 0; j < max_logo_width + 3; j++) printf(" ");
        }
        
        // Imprimir información (centrada verticalmente)
        if (info_idx >= 0 && info_idx < idx) {
            printf("%s", lines[info_idx]);
        }
        
        printf("\n");
    }
    
    printf("\n");
}

int main(int argc, char *argv[]) {
    SystemInfo info = {0};  // Inicializar toda la estructura a ceros
    
    // Inicializar explícitamente las opciones
    info.options.show_help = false;
    info.options.show_version = false;
    info.options.no_color = false;
    info.options.simple_mode = false;
    info.options.use_default_logo = false;
    
    // Procesar argumentos PRIMERO, antes de cualquier otra cosa
    parse_arguments(argc, argv, &info);
    
    // Manejar opciones que no requieren procesamiento completo
    if (info.options.show_help) {
        show_help();
        return 0;
    }
    
    if (info.options.show_version) {
        show_version();
        return 0;
    }
    
    // Configurar colores
    if (!info.options.no_color) {
        enable_ansi_colors();
    } else {
        // Si se desactivan los colores, establece todos los códigos de color a cadena vacía
        #undef RESET
        #undef BOLD
        #undef BLACK
        #undef RED
        #undef GREEN
        #undef YELLOW
        #undef BLUE
        #undef MAGENTA
        #undef CYAN
        #undef WHITE
        #undef BBLACK
        #undef BRED
        #undef BGREEN
        #undef BYELLOW
        #undef BBLUE
        #undef BMAGENTA
        #undef BCYAN
        #undef BWHITE
        
        #define RESET ""
        #define BOLD ""
        #define BLACK ""
        #define RED ""
        #define GREEN ""
        #define YELLOW ""
        #define BLUE ""
        #define MAGENTA ""
        #define CYAN ""
        #define WHITE ""
        #define BBLACK ""
        #define BRED ""
        #define BGREEN ""
        #define BYELLOW ""
        #define BBLUE ""
        #define BMAGENTA ""
        #define BCYAN ""
        #define BWHITE ""
    }
    
    // No mostrar mensaje de recopilación de información
    (void)info; // Evitar advertencia de variable no utilizada
    
    // Ahora que ya procesamos los argumentos, obtener la información del sistema
    gather_info(&info);
    
    if (info.options.simple_mode) {
        print_simple_info(&info);
    } else {
        print_info(&info);
    }
    
    return 0;
}