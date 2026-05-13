#define _CRT_SECURE_NO_WARNINGS
#define MAX_COMPUTERS 200
#define MAX_LINE 256
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include<ctype.h>

typedef struct {
    int computer_id;        
    int is_occupied;        // 是否占用 (0=空闲, 1=占用)
    char user_id[20];
    time_t start_time;      // 开始使用时间
    double current_cost;
    char area[20];          // 所在区域（普通区、电竞区、VIP区、包间区）
    double hourly_rate;     
    double max_daily_cost;  
} Computer;
Computer computers[MAX_COMPUTERS];
int computer_count = MAX_COMPUTERS;  // 实际电脑数量


typedef struct {
    char Identitycard[20];      // 身份证号
    char password[20];    // 密码
    double balance;       // 余额
    char phone[20];       // 联系电话
} Card;

typedef struct {
    double total_income;   
    int total_customers;    
    int total_minutes;      
} DailyAccount;

int Login(void);
void LoadComputerStandard(void);

int AdministratorLogin(void);
int Administrator(void);
 void CardManage(void);
  void CreateCard(void);
  void QueryCard(void);
  void RechargeCard(void);
  int CheckIDCard(const char* id);
  int IsIDCardExists(const char* identity);
 void ComputerStandardMenu(void);
  void InitComputers(void);
  void QueryComputerInfo(void);
  void ModifySingleComputer(void);
  void SaveComputerStandard(void);
 void AccountQueryMenu(void);
  void QueryTodayAccount(void);
  void QueryAccountByDate(const char* date);
  void GetCurrentDate(char* date);

int Customer(void);
 int StartComputer(void);
  int FindCardByIdentity(const char* identity, Card* card);
  int UpdateCardInFile(const Card* card);
  int FindFreeComputer(const char* area);
 int StopComputer(void);
  double CalculateCost(time_t start_time, double hourly_rate);
  void SaveToDailyAccount(double cost, int minutes);

 void QueryOnlineInfo(void);
 void ShowComputerStatus(void);

int main() {
    LoadComputerStandard();
    int x = 0, y = 1;
    while (y == 1) {
        x = Login();
        switch (x) {
        case 0:
            printf("退出程序\n");
            exit(0);
            break;
        case 1:
            y = AdministratorLogin();
            break;
        case 2:
            y = Customer();
            break;
        default:
            printf("无效选择，请重新输入\n");
            y = 1;
            break;
        }
    }
    return 0;
}
int Login(void) {
    int x;
    printf("\n========== 欢迎使用系统 ==========\n");
    printf("[1] 管理员登录\n");
    printf("[2] 顾客登录\n");
    printf("[0] 退出\n");
    printf("请选择: ");
    scanf("%d", &x);
    return x;
}


int AdministratorLogin(void) {
    char Code[] = "123456789";  
    char code[12]; 
    printf("请输入管理员密码（提示：123456789）: ");
    scanf("%s", code);

    if (strcmp(Code, code) == 0) {
        printf("登录成功！\n");
        system("pause");
        printf("\033[2J\033[H");
        Administrator();
        return 0;
    }
    else {
        printf("密码错误！请重新输入\n");
        system("pause");
        printf("\033[2J\033[H");
        return 1;
    }
}
int Administrator(void) {
    int choice;
    while (1) {
        printf("\n========== 管理员菜单 ==========\n");
        printf("[1] 卡管理系统\n");
        printf("[2] 电脑标准管理\n");
        printf("[3] 账务查询\n");
        printf("[0] 退出登录\n");
        printf("请选择: ");
        scanf("%d", &choice);

        switch (choice) {
        case 0:
            printf("退出管理员模式\n");
            return 0;
        case 1:
            printf("进入卡管理系统...\n");
            system("pause");
            printf("\033[2J\033[H");
            CardManage();
            break;
        case 2:
            printf("进入电脑标准管理...\n");
            system("pause");
            printf("\033[2J\033[H");
            ComputerStandardMenu();
            break;
        case 3:
            printf("进入账务查询...\n");
            system("pause");
            AccountQueryMenu();
            break;
        default:
            printf("无效选择！\n");
            system("pause");
            printf("\033[2J\033[H");
            break;
        }
    }
}

void CardManage(void) {
    int choice;
    while (1) {
        printf("\n========== 卡管理系统 ==========\n");
        printf("[1] 开卡\n");
        printf("[2] 查询卡信息\n");
        printf("[3] 充值\n");
        printf("[0] 返回上级菜单\n");
        printf("================================\n");
        printf("请选择: ");
        scanf("%d", &choice);

        switch (choice) {
        case 0:
            printf("返回管理员菜单...\n");
            system("pause");
            printf("\033[2J\033[H");
            return;
        case 1:
            system("pause");
            printf("\033[2J\033[H");
            CreateCard();
            break;
        case 2:
            system("pause");
            printf("\033[2J\033[H");
            QueryCard();
            break;
        case 3:
            system("pause");
            printf("\033[2J\033[H");
            RechargeCard();
            break;
        default:
            printf("无效选择！\n");
            system("pause");
        }
        printf("\033[2J\033[H");
    }
}
void CreateCard(void) {
    Card* newCard = NULL;
    char tempID[19];  // 临时存储，用于校验

    newCard = (Card*)malloc(sizeof(Card));
    if (newCard == NULL) {
        printf("内存分配失败，无法开卡！\n");
        system("pause");
        return;
    }
    memset(newCard, 0, sizeof(Card));
    printf("\n========== 开卡 ==========\n");

    // ----- 身份证号输入 + 校验位验证 -----
    printf("请输入18位身份证号: ");
    scanf("%s", tempID);

    if (!CheckIDCard(tempID)) {
        printf("身份证号校验失败（最后一位校验码错误或格式不正确）！开卡失败！\n");
        free(newCard);
        system("pause");
        return;
    }
    if (IsIDCardExists(tempID)) {
        printf("该身份证号已开卡，不能重复开卡！\n");
        free(newCard);
        system("pause");
        return;
    }
    // 校验通过后再存入结构体
    strcpy(newCard->Identitycard, tempID);

    printf("请设置密码(不长于20位数字): ");
    scanf("%s", newCard->password);

    printf("请输入联系电话: ");
    scanf("%s", newCard->phone);

    printf("请输入初始充值金额: ");
    scanf("%lf", &newCard->balance);

    if (newCard->balance < 0) {
        printf("金额不能为负数！开卡失败！\n");
        system("pause");
        return;
    }

    FILE* fp = fopen("cards.txt", "a"); 
    if (fp == NULL) {
        printf("无法打开数据文件！开卡失败！\n");
        free(newCard);
        system("pause");
        return;
    }

    fprintf(fp, "%s|%s|%.2f|%s\n",
        newCard->Identitycard,
        newCard->password,
        newCard->balance,
        newCard->phone);
    fclose(fp);

    printf("\n开卡成功！\n");
    printf("卡号: %s\n", newCard->Identitycard);
    printf("密码: %s\n", newCard->password);
    printf("余额: %.2f\n", newCard->balance);
    printf("请妥善保管卡号和密码！\n");

    free(newCard);
    system("pause");
}
void QueryCard(void) {
    char id[20];
    int found = 0;

    printf("\n========== 查询卡信息 ==========\n");
    printf("请输入身份证号: ");
    scanf("%19s", id);

    FILE* fp = fopen("cards.txt", "r");
    if (fp == NULL) {
        printf("暂无数据！\n");
        system("pause");
        return;
    }

    char line[256];
    Card card;

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%19[^|]|%19[^|]|%lf|%19s", //[^|]表示遇到|就停止读入
            card.Identitycard, card.password,
            &card.balance, card.phone) == 4) {
            if (strcmp(card.Identitycard, id) == 0) {
                printf("\n身份证号: %s\n", card.Identitycard);
                printf("密码: %s\n", card.password);
                printf("余额: %.2f 元\n", card.balance);
                printf("电话: %s\n", card.phone);
                found = 1;
                break;
            }
        }
    }

    fclose(fp);

    if (!found) {
        printf("未找到该卡信息！\n");
    }

    system("pause");
}
void RechargeCard(void) {
    char id[20];
    double amount;
    int found = 0;
    
    printf("\n========== 卡充值 ==========\n");
    printf("请输入要充值的身份证号: ");
    scanf("%19s", id);
    
    printf("请输入充值金额: ");
    scanf("%lf", &amount);
    
    if (amount <= 0) {
        printf("充值金额必须大于0！\n");
        system("pause");
        return;
    }
    
    FILE* fp = fopen("cards.txt", "r");
    if (fp == NULL) {
        printf("暂无卡数据！请先开卡。\n");
        system("pause");
        return;
    }
    
    FILE* temp_fp = fopen("temp.txt", "w");
    if (temp_fp == NULL) {
        printf("系统错误！\n");
        fclose(fp);
        system("pause");
        return;
    }
    
    char line[256];
    char temp_id[20], temp_pwd[20], temp_phone[20];
    double temp_balance;
    
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%19[^|]|%19[^|]|%lf|%19s", 
                   temp_id, temp_pwd, &temp_balance, temp_phone) == 4) {
            
            if (strcmp(temp_id, id) == 0) {
                temp_balance += amount;
                printf("\n充值成功！新余额: %.2f 元\n", temp_balance);
                found = 1;
            }
            
            // 写入临时文件
            fprintf(temp_fp, "%s|%s|%.2f|%s\n", 
                   temp_id, temp_pwd, temp_balance, temp_phone);
        }
    }
    
    fclose(fp);
    fclose(temp_fp);
    
    if (found) {
        remove("cards.txt");
        rename("temp.txt", "cards.txt");
    } else {
        remove("temp.txt");
        printf("未找到该卡！\n");
    }
    
    system("pause");
}
int CheckIDCard(const char* id) {
    // 长度必须是18位
    if (strlen(id) != 18) return 0;

    // 加权因子
    const int weight[] = { 7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2 };
    // 校验码对应字符
    const char checkCode[] = "10X98765432";

    int sum = 0;
    // 前17位必须全是数字
    for (int i = 0; i < 17; i++) {
        if (!isdigit(id[i])) return 0;
        sum += (id[i] - '0') * weight[i];
    }

    int mod = sum % 11;
    char lastChar = id[17];
    // 最后一位允许大写X或小写x
    if (lastChar == 'x') lastChar = 'X';

    return (lastChar == checkCode[mod]);
}
int IsIDCardExists(const char* identity) {
    FILE* fp = fopen("cards.txt", "r");
    if (fp == NULL) return 0;

    char line[MAX_LINE];
    char temp_id[20];

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%19[^|]", temp_id);
        if (strcmp(temp_id, identity) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void ComputerStandardMenu(void) {
    int choice;

    while (1) {
        printf("\n========== 电脑标准管理 ==========\n");
        printf("[1] 查询电脑状态\n");
        printf("[2] 修改电脑标准\n");
        printf("[3] 恢复默认设置\n");
        printf("[4] 保存\n");
        printf("[0] 返回上级菜单\n");
        printf("==================================\n");
        printf("请选择: ");
        scanf("%d", &choice);

        switch (choice) {
        case 0:
            printf("\033[2J\033[H");
            return;
        case 1:
            system("pause");
            printf("\033[2J\033[H");
            QueryComputerInfo();
            break;
        case 2:
            system("pause");
            printf("\033[2J\033[H");
            ModifySingleComputer();
            break;
        case 3:
            InitComputers();
            printf("已恢复默认计费标准！\n");
            system("pause");
            printf("\033[2J\033[H");
            break;
        case 4:
            SaveComputerStandard();
            printf("已保存！\n");
            system("pause");
            printf("\033[2J\033[H");
            break;
        default:
            printf("无效选择！\n");
            system("pause");
            printf("\033[2J\033[H");
            break;
        }
    }
}
void SaveComputerStandard(void) {
    FILE* fp = fopen("computer_standard.dat", "wb");
    if (fp == NULL) {
        printf("无法保存电脑标准！\n");
        return;
    }

    fwrite(computers, sizeof(Computer), MAX_COMPUTERS, fp);
    fclose(fp);

    printf("电脑标准已保存到文件 computer_standard.dat！\n");
}
void ModifySingleComputer(void) {
    int computer_id;
    double new_rate;
    double new_max_cost;

    printf("\n========== 修改单个电脑标准 ==========\n");
    printf("请输入要修改的电脑编号 (1-%d): ", MAX_COMPUTERS);
    scanf("%d", &computer_id);

    if (computer_id < 1 || computer_id > MAX_COMPUTERS) {
        printf("电脑编号无效！\n");
        system("pause");
        return;
    }

    int index = computer_id - 1;

    printf("\n当前电脑信息：\n");
    printf("电脑编号: %d\n", computers[index].computer_id);
    printf("所在区域: %s\n", computers[index].area);
    printf("当前费率: %.2f 元/小时\n", computers[index].hourly_rate);
    printf("当前封顶: %.2f 元/天\n", computers[index].max_daily_cost);

    printf("\n请输入新的小时费率 (元/小时): ");
    scanf("%lf", &new_rate);
    if (new_rate >= 0) {
        computers[index].hourly_rate = new_rate;
    }
    else {
        printf("费率不能为负数，保持原值！\n");
    }

    printf("请输入新的每日封顶金额 (元): ");
    scanf("%lf", &new_max_cost);
    if (new_max_cost >= 0) {
        computers[index].max_daily_cost = new_max_cost;
    }
    else {
        printf("封顶金额不能为负数，保持原值！\n");
    }

    printf("\n修改成功！新计费标准：\n");
    printf("电脑 %d: %.2f 元/小时，每日封顶 %.2f 元\n",
        computer_id,
        computers[index].hourly_rate,
        computers[index].max_daily_cost);

    system("pause");
}
void InitComputers(void) {
    computer_count = MAX_COMPUTERS;

    for (int i = 0; i < MAX_COMPUTERS; i++) {
        computers[i].computer_id = i + 1;           
        computers[i].is_occupied = 0;               
        strcpy(computers[i].user_id, "");           
        computers[i].start_time = 0;                
        computers[i].current_cost = 0.0;           

        if (i < 50) {  // 1-50号：普通区
            strcpy(computers[i].area, "普通区");
            computers[i].hourly_rate = 3.0;
            computers[i].max_daily_cost = 50.0;
        }
        else if (i < 100) {  // 51-100号：电竞区
            strcpy(computers[i].area, "电竞区");
            computers[i].hourly_rate = 5.0;
            computers[i].max_daily_cost = 80.0;
        }
        else if (i < 150) {  // 101-150号：VIP区
            strcpy(computers[i].area, "VIP区");
            computers[i].hourly_rate = 8.0;
            computers[i].max_daily_cost = 100.0;
        }
        else {  // 151-200号：包间区
            strcpy(computers[i].area, "包间区");
            computers[i].hourly_rate = 10.0;
            computers[i].max_daily_cost = 150.0;
        }
    }

    printf("已初始化 %d 台电脑信息\n", MAX_COMPUTERS);
}
void QueryComputerInfo(void) {
    int computer_id;

    printf("\n========== 查询电脑信息 ==========\n");
    printf("请输入电脑编号 (1-%d): ", MAX_COMPUTERS);
    scanf("%d", &computer_id);

    if (computer_id < 1 || computer_id > MAX_COMPUTERS) {
        printf("电脑编号无效！\n");
        system("pause");
        return;
    }

    int index = computer_id - 1;

    printf("\n========== 电脑 %d 详细信息 ==========\n", computer_id);
    printf("电脑编号: %d\n", computers[index].computer_id);
    printf("所在区域: %s\n", computers[index].area);
    printf("小时费率: %.2f 元/小时\n", computers[index].hourly_rate);
    printf("每日封顶: %.2f 元\n", computers[index].max_daily_cost);
    printf("当前状态: %s\n", computers[index].is_occupied ? "占用中" : "空闲");

    if (computers[index].is_occupied) {
        printf("使用者ID: %s\n", computers[index].user_id);

        // 计算已使用时间
        time_t now = time(NULL);
        int used_minutes = (int)difftime(now, computers[index].start_time) / 60;
        int hours = used_minutes / 60;
        int minutes = used_minutes % 60;

        printf("开始时间: %s", ctime(&computers[index].start_time));
        printf("已用时间: %d小时%d分钟\n", hours, minutes);
        printf("当前费用: %.2f 元\n", computers[index].current_cost);
    }
    printf("======================================\n");

    system("pause");
}

void AccountQueryMenu(void) {
    int choice;
    char date[20];

    while (1) {
        printf("\n========== 账务查询 ==========\n");
        printf("[1] 查询今日账务\n");
        printf("[2] 查询指定日期\n");
        printf("[0] 返回上级菜单\n");
        printf("==============================\n");
        printf("请选择: ");
        scanf("%d", &choice);

        switch (choice) {
        case 0:
            return;
        case 1:
            QueryTodayAccount();
            break;
        case 2:
            printf("请输入日期(格式: 2024-01-15): ");
            scanf("%s", date);
            QueryAccountByDate(date);
            break;
        default:
            printf("无效选择！\n");
            system("pause");
            break;
        }
        printf("\033[2J\033[H");
    }
}
void GetCurrentDate(char* date) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    sprintf(date, "%04d%02d%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
}
void QueryTodayAccount(void) {
    char date[20];
    GetCurrentDate(date);
    QueryAccountByDate(date);
}
void QueryAccountByDate(const char* date) {
    char filename[50];
    sprintf(filename, "account_%s.dat", date);

    DailyAccount account;
    FILE* fp = fopen(filename, "rb");

    printf("\n========== %s 账务 ==========\n", date);

    if (fp == NULL) {
        printf("该日期没有营业数据\n");
    }
    else {
        size_t bytes_read = fread(&account, sizeof(DailyAccount), 1, fp);
        fclose(fp);

        if (bytes_read != 1) {
            printf("读取账单数据失败！\n");
        }
        else {
            printf("总收入:     %.2f 元\n", account.total_income);
            printf("总顾客数:   %d 人\n", account.total_customers);
            printf("总时长:     %d 分钟 (%.2f 小时)\n",
                account.total_minutes, account.total_minutes / 60.0);

            // 避免除零错误
            if (account.total_customers > 0) {
                printf("人均消费:   %.2f 元\n", account.total_income / account.total_customers);
                printf("平均时长:   %.2f 分钟\n", (double)account.total_minutes / account.total_customers);
            }
            else {
                printf("人均消费:   0.00 元\n");
                printf("平均时长:   0.00 分钟\n");
            }
        }
    }
    printf("==============================\n");

    system("pause");
}



int Customer(void) {
    int choice;

    while (1) {
        system("cls");
        printf("\n========== 网吧计费系统 - 顾客菜单 ==========\n");
        printf("1. 上机\n");
        printf("2. 下机\n");
        printf("3. 查询上机信息\n");
        printf("4. 查看电脑状态\n");
        printf("0. 返回主菜单\n");
        printf("==========================================\n");
        printf("请选择: ");

        scanf("%d", &choice);

        switch (choice) {
        case 1: StartComputer(); break;
        case 2: StopComputer(); break;
        case 3: QueryOnlineInfo(); break;
        case 4: ShowComputerStatus(); system("pause"); break;
        case 0:
            printf("退出...\n");
            return 0;
        default:
            printf("无效选择，请重新输入！\n");
            system("pause");
        }
    }
    return 0;
}
int StartComputer(void) {
    char identity[20], password[20], area[20];
    int computer_index;
    Card current_card;

    printf("\n========== 顾客上机 ==========\n");

    // 1. 输入身份证号和密码
    printf("请输入身份证号: ");
    scanf("%s", identity);
    printf("请输入密码: ");
    scanf("%s", password);

    // 2. 从文件验证身份
    if (!FindCardByIdentity(identity, &current_card)) {
        printf("身份证号不存在，请先开卡！\n");
        system("pause");
        return 0;
    }

    if (strcmp(current_card.password, password) != 0) {
        printf("密码错误！\n");
        system("pause");
        return 0;
    }

    // 3. 检查余额
    if (current_card.balance <= 0) {
        printf("余额不足，请充值！当前余额: %.2f元\n", current_card.balance);
        system("pause");
        return 0;
    }

    // 4. 选择区域
    printf("\n请选择上机区域:\n");
    printf("1. 普通区 (3元/小时)\n");
    printf("2. 电竞区 (5元/小时)\n");
    printf("3. VIP区 (8元/小时)\n");
    printf("4. 包间区 (10元/小时)\n");
    printf("请选择(1-4): ");

    int area_choice;
    scanf("%d", &area_choice);

    switch (area_choice) {
    case 1: strcpy(area, "普通区"); break;
    case 2: strcpy(area, "电竞区"); break;
    case 3: strcpy(area, "VIP区"); break;
    case 4: strcpy(area, "包间区"); break;
    default:
        printf("无效选择！\n");
        system("pause");
        return 0;
    }

    // 5. 查找空闲电脑（需要实现全局computers数组）
    computer_index = FindFreeComputer(area);
    if (computer_index == -1) {
        printf("抱歉，当前区域没有空闲电脑！\n");
        system("pause");
        return 0;
    }

    // 6. 上机操作
    computers[computer_index].is_occupied = 1;
    strcpy(computers[computer_index].user_id, identity);
    computers[computer_index].start_time = time(NULL);
    computers[computer_index].current_cost = 0;

    // 根据区域设置费率
    switch (area_choice) {
    case 1: computers[computer_index].hourly_rate = 3.0; break;
    case 2: computers[computer_index].hourly_rate = 5.0; break;
    case 3: computers[computer_index].hourly_rate = 8.0; break;
    case 4: computers[computer_index].hourly_rate = 10.0; break;
    }

    printf("\n上机成功！\n");
    printf("电脑编号: %d\n", computers[computer_index].computer_id);
    printf("所在区域: %s\n", computers[computer_index].area);
    printf("收费标准: %.2f元/小时\n", computers[computer_index].hourly_rate);
    printf("当前余额: %.2f元\n", current_card.balance);
    printf("祝您上网愉快！\n");

    system("pause");
    return 1;
}
int FindCardByIdentity(const char* identity, Card* card) {
    FILE* fp = fopen("cards.txt", "r");
    if (fp == NULL) {
        printf("无法打开数据文件！\n");
        return 0;
    }

    char line[MAX_LINE];
    char temp_identity[20], temp_password[20], temp_phone[20];
    double temp_balance;

    // 逐行读取文件
    while (fgets(line, sizeof(line), fp)) {
        // 去除换行符
        line[strcspn(line, "\n")] = 0;

        // 解析格式: 身份证号|密码|余额|电话
        char* token = strtok(line, "|");
        if (token == NULL) continue;
        strcpy(temp_identity, token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(temp_password, token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        temp_balance = atof(token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(temp_phone, token);

        // 比较身份证号
        if (strcmp(temp_identity, identity) == 0) {
            // 找到卡信息，复制到card结构体中
            strcpy(card->Identitycard, temp_identity);
            strcpy(card->password, temp_password);
            card->balance = temp_balance;
            strcpy(card->phone, temp_phone);

            fclose(fp);
            return 1;  // 找到
        }
    }

    fclose(fp);
    return 0;  // 未找到
}
int UpdateCardInFile(const Card* card) {
    FILE* fp = fopen("cards.txt", "r");
    if (fp == NULL) {
        printf("无法打开数据文件！\n");
        return 0;
    }

    // 读取所有卡信息到临时数组
    Card temp_cards[1000];  // 假设最多1000张卡
    int card_count = 0;
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), fp) && card_count < 1000) {
        line[strcspn(line, "\n")] = 0;

        char* token = strtok(line, "|");
        if (token == NULL) continue;
        strcpy(temp_cards[card_count].Identitycard, token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(temp_cards[card_count].password, token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        temp_cards[card_count].balance = atof(token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(temp_cards[card_count].phone, token);

        card_count++;
    }
    fclose(fp);

    // 查找并更新目标卡
    int found = 0;
    for (int i = 0; i < card_count; i++) {
        if (strcmp(temp_cards[i].Identitycard, card->Identitycard) == 0) {
            // 更新信息
            strcpy(temp_cards[i].password, card->password);
            temp_cards[i].balance = card->balance;
            strcpy(temp_cards[i].phone, card->phone);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("未找到要更新的卡信息！\n");
        return 0;
    }

    // 重新写入文件
    fp = fopen("cards.txt", "w");
    if (fp == NULL) {
        printf("无法写入数据文件！\n");
        return 0;
    }

    for (int i = 0; i < card_count; i++) {
        fprintf(fp, "%s|%s|%.2f|%s\n",
            temp_cards[i].Identitycard,
            temp_cards[i].password,
            temp_cards[i].balance,
            temp_cards[i].phone);
    }
    fclose(fp);

    return 1;
}
int FindFreeComputer(const char* area) {
    for (int i = 0; i < MAX_COMPUTERS; i++) {
        if (computers[i].is_occupied == 0 &&
            (area == NULL || strcmp(computers[i].area, area) == 0)) {
            return i;
        }
    }
    return -1;
}
int StopComputer(void) {
    char identity[20], password[20];
    int found = -1;
    Card current_card;  // 改为局部变量，不用指针
    double cost;

    printf("\n========== 顾客下机 ==========\n");

    // 1. 输入身份证号和密码
    printf("请输入身份证号: ");
    scanf("%s", identity);
    printf("请输入密码: ");
    scanf("%s", password);

    // 2. 从文件中验证身份
    if (!FindCardByIdentity(identity, &current_card)) {
        printf("身份证号不存在！\n");
        system("pause");
        return 0;
    }

    if (strcmp(current_card.password, password) != 0) {
        printf("密码错误！\n");
        system("pause");
        return 0;
    }

    // 3. 查找正在使用的电脑
    for (int i = 0; i < computer_count; i++) {
        if (computers[i].is_occupied == 1 &&
            strcmp(computers[i].user_id, identity) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("您当前没有正在使用的电脑！\n");
        system("pause");
        return 0;
    }

    // 4. 计算费用和使用时间
    cost = CalculateCost(computers[found].start_time,
        computers[found].hourly_rate);

    // 最低消费1元
    if (cost < 1.0 && cost > 0) {
        cost = 1.0;
    }

    // 计算使用分钟数
    time_t now = time(NULL);
    int total_minutes = (int)difftime(now, computers[found].start_time) / 60;
    int hours = total_minutes / 60;
    int minutes = total_minutes % 60;

    // 5. 检查余额是否足够
    if (current_card.balance < cost) {
        printf("余额不足！\n");
        printf("应收: %.2f元, 余额: %.2f元, 差额: %.2f元\n",
            cost, current_card.balance, cost - current_card.balance);
        printf("请充值后再下机！\n");
        system("pause");
        return 0;
    }

    // 6. 扣费并更新信息
    current_card.balance -= cost;

    // 更新文件中的卡余额
    if (!UpdateCardInFile(&current_card)) {
        printf("更新卡信息失败！\n");
        system("pause");
        return 0;
    }

    // 7. 保存到当天账单
    SaveToDailyAccount(cost, total_minutes);

    // 8. 记录到电脑的当前消费
    computers[found].current_cost = cost;

    // 9. 显示账单
    printf("\n========== 消费账单 ==========\n");
    printf("身份证号: %s\n", identity);
    printf("电脑编号: %d\n", computers[found].computer_id);
    printf("所在区域: %s\n", computers[found].area);
    printf("上机时间: %s", ctime(&computers[found].start_time));
    printf("下机时间: %s", ctime(&now));
    printf("使用时长: %d小时%d分钟\n", hours, minutes);
    printf("收费标准: %.2f元/小时\n", computers[found].hourly_rate);
    printf("消费金额: %.2f元\n", cost);
    printf("剩余余额: %.2f元\n", current_card.balance);
    printf("==============================\n");

    // 10. 释放电脑
    computers[found].is_occupied = 0;
    memset(computers[found].user_id, 0, sizeof(computers[found].user_id));
    computers[found].start_time = 0;
    computers[found].current_cost = 0;

    printf("\n下机成功，欢迎下次光临！\n");
    system("pause");
    return 1;
}
double CalculateCost(time_t start_time, double hourly_rate) {
    time_t now = time(NULL);
    double hours = difftime(now, start_time) / 3600.0;
    double cost = hours * hourly_rate;

    // 保留两位小数
    return (int)(cost * 100) / 100.0;
}
void SaveToDailyAccount(double cost, int minutes) {
    // 获取当前日期
    time_t now = time(NULL);
    struct tm* t = localtime(&now);

    char filename[50];
    sprintf(filename, "account_%04d%02d%02d.dat",
        t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);

    DailyAccount account;
    FILE* fp = fopen(filename, "rb");

    if (fp == NULL) {
        // 文件不存在，初始化新账单
        memset(&account, 0, sizeof(DailyAccount));  // 先清零
        account.total_income = cost;
        account.total_customers = 1;
        account.total_minutes = minutes;
    }
    else {
        // 读取现有账单
        size_t bytes_read = fread(&account, sizeof(DailyAccount), 1, fp);
        fclose(fp);

        if (bytes_read != 1) {
            // 读取失败，初始化
            memset(&account, 0, sizeof(DailyAccount));
            account.total_income = cost;
            account.total_customers = 1;
            account.total_minutes = minutes;
        }
        else {
            // 更新账单
            account.total_income += cost;
            account.total_customers++;
            account.total_minutes += minutes;
        }
    }

    // 写入文件
    fp = fopen(filename, "wb");
    if (fp != NULL) {
        fwrite(&account, sizeof(DailyAccount), 1, fp);
        fclose(fp);
    }
}
void QueryOnlineInfo(void) {
    char identity[20];

    printf("\n========== 查询上机信息 ==========\n");
    printf("请输入身份证号: ");
    scanf("%s", identity);

    int found = -1;
    for (int i = 0; i < computer_count; i++) {
        if (computers[i].is_occupied == 1 &&
            strcmp(computers[i].user_id, identity) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("您当前没有正在使用的电脑！\n");
        system("pause");
        return;
    }

    time_t now = time(NULL);
    double hours = difftime(now, computers[found].start_time) / 3600.0;
    double current_cost = hours * computers[found].hourly_rate;

    printf("\n当前上机信息:\n");
    printf("电脑编号: %d\n", computers[found].computer_id);
    printf("所在区域: %s\n", computers[found].area);
    printf("已上机时间: %.2f小时\n", hours);
    printf("当前费用: %.2f元\n", current_cost);
    printf("费率: %.2f元/小时\n", computers[found].hourly_rate);

    system("pause");
}
void ShowComputerStatus(void) {
    int i;
    printf("请输入希望查询的电脑编号(1-200):");
    scanf("%d", &i);
    if (i <= 0 || i > 200) {
        printf("您输入的电脑编号不存在！");
        exit(0);
    }
    printf("\n========== 电脑状态列表 ==========\n");
    printf("%-6s %-10s %-8s %-10s %-10s\n",
        "机号", "区域", "状态", "当前费用", "用户ID");
    printf("----------------------------------------\n");


    printf("%-6d %-10s %-8s %-10.2f %-10s\n",
            computers[i].computer_id,
            computers[i].area,
            computers[i].is_occupied ? "使用中" : "空闲",
            computers[i].current_cost,
            computers[i].is_occupied ? computers[i].user_id : "-");
    printf("========================================\n");
}


void LoadComputerStandard(void) {
    FILE* fp = fopen("computer_standard.dat", "rb");
    if (fp == NULL) {
        printf("没有保存的电脑标准，使用默认配置。\n");
        InitComputers();
        return;
    }

    fread(computers, sizeof(Computer), MAX_COMPUTERS, fp);
    fclose(fp);

    printf("电脑标准加载成功！\n");
}
