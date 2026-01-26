#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Structure to store appliance information */
struct Appliance {
    char name[50];
    int watt;
    int quantity;
    float hours;
};

/* Load appliances from file */
void loadAppliances(struct Appliance *arr, int count)
{
    FILE *fp = fopen("appliances.txt", "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fscanf(fp, " %[^,],%d", arr[i].name, &arr[i].watt);
    }

    fclose(fp);
}

/* Get usage from user */
void getUsageFromUser(struct Appliance *arr, int count)
{
    for (int i = 0; i < count; i++) {
        printf("\nAppliance: %s (%dW)\n", arr[i].name, arr[i].watt);

        printf("Quantity: ");
        scanf("%d", &arr[i].quantity);

        printf("Hours per day: ");
        scanf("%f", &arr[i].hours);
    }
}

/* Calculate daily energy in Wh and print in table */
float calculateDailyEnergy(struct Appliance *arr, int count)
{
    float totalWh = 0;

    /* Print table header */
    printf("\n%-30s %6s %6s %7s %12s\n", "Appliance Name", "Watt", "Qty", "Hours", "Energy(Wh)");
    printf("-------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        float applianceWh = arr[i].watt * arr[i].quantity * arr[i].hours;
        totalWh += applianceWh;

        printf("%-30s %6d %6d %7.2f %12.2f\n",
               arr[i].name, arr[i].watt,
               arr[i].quantity, arr[i].hours, applianceWh);
    }

    printf("-------------------------------------------------------------------------------\n");
    printf("%-30s %6s %6s %7s %12.2f\n", "TOTAL", "", "", "", totalWh);

    return totalWh;
}

/* Convert Wh to kWh */
float convertWhToKWh(float totalWh)
{
    float totalKWh = totalWh / 1000.0;
    printf("\nTotal energy in kWh: %.2f kWh\n", totalKWh);
    return totalKWh;
}

/* Calculate recommended inverter size */
float calculateInverterFromWh(float totalWh)
{
    float avgLoad = totalWh / 24.0;
    float inverterVA = avgLoad * 1.25;

    printf("Recommended inverter size: %.0f VA\n", inverterVA);
    return inverterVA;
}

/* Calculate required number of solar panels */
int calculateSolarPanels(float totalWh)
{
    float peakSunHours = 4.5;   /* Teacher-provided peak sun hours */
    float pvSize = (totalWh / peakSunHours) * 1.3;

    int numPanels = (int)(pvSize / 585 + 0.999);
    printf("Recommended solar panels: %d * 585W\n", numPanels);
    return numPanels;
}

/* Display battery recommendations in table */
void displayBatteryBank(float totalWh)
{
    /* Lead Acid constants */
    const int voltageLA = 12;
    const int AhLA = 200;
    const float DoD_LA = 0.5;       // 50%
    const int cyclesLA_min = 2000;
    const int cyclesLA_max = 3000;

    /* Lithium constants */
    const int voltageLi = 12;
    const int AhLi = 200;
    const float DoD_Li = 0.8;       // 80%
    const int cyclesLi_min = 6000;
    const int cyclesLi_max = 10000;

    /* Lead Acid calculation */
    float requiredAhLA = (totalWh / voltageLA) / DoD_LA;
    int numBatteriesLA = (int)ceil(requiredAhLA / AhLA);
    float totalCapacityLA_kWh = (numBatteriesLA * AhLA * voltageLA) / 1000.0; // kWh

    /* Lithium calculation */
    float requiredAhLi = (totalWh / voltageLi) / DoD_Li;
    int numBatteriesLi = (int)ceil(requiredAhLi / AhLi);
    float totalCapacityLi_kWh = (numBatteriesLi * AhLi * voltageLi) / 1000.0; // kWh

    /* Print battery table header */
    printf("\n%-15s %15s %20s %7s %20s\n",
           "Battery Type", "No. Batteries", "Total Capacity(kWh)", "DoD", "Estimated Life (Cycles)");
    printf("-----------------------------------------------------------------------------------------------\n");

    /* Print Lead Acid */
    printf("%-15s %15d %20.2f %7s %20d-%d\n",
           "Lead Acid", numBatteriesLA, totalCapacityLA_kWh, "50%", cyclesLA_min, cyclesLA_max);

    /* Print Lithium */
    printf("%-15s %15d %20.2f %7s %20d-%d\n",
           "Lithium", numBatteriesLi, totalCapacityLi_kWh, "80%", cyclesLi_min, cyclesLi_max);
    printf("-----------------------------------------------------------------------------------------------\n");
}

int main()
{
    FILE *fp;
    int count = 0;
    char line[100];

    float totalWh, totalKWh, inverterVA;
    int panelCount;

    /* Open file to count number of appliances */
    fp = fopen("appliances.txt", "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    /* Count number of lines (appliances) */
    while (fgets(line, sizeof(line), fp) != NULL) {
        count++;
    }
    fclose(fp);

    /* Allocate memory for appliances */
    struct Appliance *list = malloc(count * sizeof(struct Appliance));
    if (list == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    /* Call functions */
    loadAppliances(list, count);
    getUsageFromUser(list, count);

    totalWh = calculateDailyEnergy(list, count);
    totalKWh = convertWhToKWh(totalWh);
    inverterVA = calculateInverterFromWh(totalWh);
    panelCount = calculateSolarPanels(totalWh);

    /* Display battery recommendations in table */
    displayBatteryBank(totalWh);

    /* Free allocated memory */
    free(list);

    return 0;
}
