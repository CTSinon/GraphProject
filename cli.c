#include "cli.h"
#include "stats.h"
#include <stdio.h>
#include <stdlib.h>
#include "search.h"
#include "tools.h"


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/* global variables for nessessary values used by calculate */
static char *filename;
static int startPoint;
static int targetPoint;

static void throwErr(void);

/* function implementation for corresponding flags */
static void helperFunc(void);
static void freemanFunc(void);
static void edgesFunc(void);
static void verticesFunc(void);
static void closenessFunc(void);
static void dfsFunc(void);
static void bfsFunc(void);
static void dijkstraFunc(void);
static void logoFunc(void);

static void logoFunc() {
        puts(ANSI_COLOR_YELLOW
" _________________________\n"
"< 想不到吧，蒜头君是一头牛 >\n"
" -------------------------\n"
ANSI_COLOR_GREEN
"        \\   ^__^\n"
ANSI_COLOR_CYAN
"         \\  (oo)\\_______\n"
"            (__)\\       )\\/\n"
ANSI_COLOR_MAGENTA
"                ||----w |\n"
"                ||     ||\n"
     ANSI_COLOR_RESET "\n");
}


static void helperFunc() {
    const char *helpMesg = 
        "Usage: search-cli  [OPTION]... [FILE]... \n"
"This program can help you analyse a graph specified by an input file. \n"
" \n"
"Mandatory arguments to long options are mandatory for short options too.\n"
"  -h, --help                   to show the helping list. \n"
"  -g, --graph <FILE_PATH>      to open the graph based on the file. \n"
"  -s, --stats <STATS_PARAMS>   show the corresponding statistics data of this graph. \n"
"               edges           show the number of edges in graph. \n"
"               vertices        show the number of vertices in graph. \n"
"               freeman         show the Freeman's Network Centrality. \n"
"               closeness       show the closeness centrality of the point. \n"
"  -sp, --shortestpath          calculate the shortest between two points in the graph. \n"
"  -u <STARTING_POINT>          start from the specific point, only available with -sp. \n"
"  -v <TARGET_POINT>            specify the target point, only available with -sp. \n";
    puts(helpMesg);
}

static void freemanFunc() {
    printf("Reading %s ...\n", filename);
    float res = freemanNetworkCentrality(filename);
    printf("Freeman's Network Centrality is %f.\n", res);
}

static void edgesFunc() {
    printf("Reading %s ...\n", filename);
    int res = numberOfEdges(filename);
    printf("There are %d edges in this graph.\n", res);
}

static void verticesFunc() {
    printf("Reading %s ...\n", filename);
    int res = numberOfVertices(filename);
    printf("There are %d vertices in this graph.\n", res);
}

static void closenessFunc() {
    printf("Reading %s ...\n", filename);
    float res = closenessCentrality(filename, startPoint);
    printf("Closeness Centrality is %f.\n", res);
}

static void dfsFunc(void) {
    int *res = NULL;
    res = graphDFS(filename, startPoint, targetPoint);
    int len = 0;
    for ( ; res[len + 1] != -1; ++len);
    if (len == 0) {
        printf("No path.\n");
        free(res);
        return;
    }
    printf("Path found:\n");
    for (int i = 0; i < len; ++i) {
        printf("%d -> ", res[i]);
    }
    printf("%d\n", res[len]);
    free(res);
    return;
}

static void bfsFunc(void) {
    int *res = NULL;
    res = graphBFS(filename, startPoint, targetPoint);
    int len = 0;
    for ( ; res[len + 1] != -1; ++len);
    if (len == 0) {
        printf("No path.\n");
        free(res);
        return;
    }
    printf("Cost: %llu\n", *(unsigned long long*)(&res[len + 2]));
    printf("Path found:\n");
    for ( ; len > 0; --len) {
        printf("%d -> ", res[len]);
    }
    printf("%d\n", res[0]);
    free(res);
}

static void dijkstraFunc(void) {
    int *res = NULL;
    res = graphDijkstar(filename, startPoint, targetPoint);
    int len = 0;
    for ( ; res[len + 1] != -1; ++len);
    if (len == 0) {
        printf("No path.\n");
        free(res);
        return;
    }
    printf("Cost: %llu\n", *(unsigned long long*)(&res[len + 2]));
    printf("Path found:\n");
    for ( ; len > 0; --len) {
        printf("%d -> ", res[len]);
    }
    printf("%d\n", res[0]);
    free(res);
}

static void throwErr() {
    char *argErr = 
    ANSI_COLOR_RED
    "Argument Error."
    ANSI_COLOR_RESET;
    puts(argErr);
    exit(-1);
}

void (*parseArguments(int argc, char **argv))(void) {
    int fileFlag = 0;
    int spFlag = 0;
    int statFlag = 0;
    int startFlag = 0;
    int endFlag = 0;
    char *statMode;
    char *searchParams;
    /* we will scan all arguments and try to set the corresponding flag specified by commandline */
    
    if (argc == 1) {
        throwErr();
    }
    
    for (int i = 1; i < argc; ) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            return helperFunc;
        } else if (strcmp(argv[i], "-j") == 0) {
            return logoFunc;
        } else if (strcmp(argv[i], "-sp") == 0 || strcmp(argv[1], "--shortestpath") == 0) {
            spFlag = 1;
            searchParams = argv[i + 1];
            i += 2;
        } else if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--graph") == 0) {
            if (argc <= i + 1) {
                throwErr();
            }
            FILE *tmp = NULL;
            if (!(tmp = fopen(argv[i + 1], "r"))) {
                throwErr();
            }
            fileFlag = 1;
            fclose(tmp);
            filename = argv[i + 1];
            i += 2;
        } else if (strcmp(argv[i], "-u") == 0) {
            if (argc <= i + 1) {
                throwErr();
            }
            for (int j = 0; argv[i + 1][j] != 0; ++j) {
                if (argv[i + 1][j] < '0' || argv[i + 1][j] > '9') {
                    throwErr();
                }
                startPoint = atoi(argv[i + 1]);
                startFlag = 1;
            }
            i += 2;
        } else if (strcmp(argv[i], "-v") == 0) {
            if (argc <= i + 1) {
                throwErr();
            }
            for (int j = 0; argv[i + 1][j] != 0; ++j) {
                if (argv[i + 1][j] < '0' || argv[i + 1][j] > '9') {
                    throwErr();
                }
                targetPoint = atoi(argv[i + 1]);
                endFlag = 1;
            }
            i += 2;
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--stats") == 0) {
            if (argc <= i + 1) {
                throwErr();
            }
            statMode = argv[i + 1];
            if (strcmp(statMode, "closeness") == 0) {
                if (argc <= i + 2) {
                    throwErr();
                }
                for (int j = 0; argv[i + 2][j] != 0; ++j) {
                    if (argv[i + 2][j] < '0' || argv[i + 2][j] > '9') {
                        throwErr();
                    }
                }
                startPoint = atoi(argv[i + 2]);
                startFlag = 1;
                i++;
            }
            statFlag = 1;
            i += 2;
        } else {
            throwErr();
        }
    }
    
    if (statFlag) {
        if (!fileFlag) {
            throwErr();
        }
        if (strcmp(statMode, "freeman") == 0) {
            return freemanFunc;
        } else if (strcmp(statMode, "vertices") == 0) {
            return verticesFunc;
        } else if (strcmp(statMode, "edges") == 0) {
            return edgesFunc;
        } else if (strcmp(statMode, "closeness") == 0) {
            if (!startFlag) {
                throwErr();
            }
            return closenessFunc;
        } else {
            throwErr();
        }
    }

    if (spFlag) {
        if (!(fileFlag && startFlag && endFlag)) {
            throwErr();
        }
        if (strcmp(searchParams, "Dijkstra") == 0) {
            return dijkstraFunc;
        } else if (strcmp(searchParams, "BFS") == 0) {
            return bfsFunc;
        } else if (strcmp(searchParams, "DFS") == 0) {
            return dfsFunc;
        } else {
            throwErr();
        }
    }
    throwErr();
    return NULL;
}