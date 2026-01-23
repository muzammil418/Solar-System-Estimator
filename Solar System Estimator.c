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

/* Create a function to load appliances into the structure array */
void loadAppliances(struct Appliance *arr, int count)
{
    /* Open the file */
    FILE *fp = fopen("appliances.txt", "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    /* Use a loop to read appliance data from the file */
    for (int i = 0; i < count; i++) {
        fscanf(fp, " %[^,],%d", arr[i].name, &arr[i].watt);
    }

    fclose(fp);
}

/* Create a function to get quantity and usage hours from the user */
void getUsageFromUser(struct Appliance *arr, int count)
{
    /* Use a loop to get user input for each appliance */
    for (int i = 0; i < count; i++) {
        printf("\nAppliance: %s (%dW)\n", arr[i].name, arr[i].watt);

        printf("Quantity: ");
        scanf("%d", &arr[i].quantity);

        printf("Hours per day: ");
        scanf("%f", &arr[i].hours);
    }
}

/* Create a function to calculate daily energy consumption in watt-hours */
float calculateDailyEnergy(struct Appliance *arr, int count)
{
    /* Create a variable to store total watt-hours */
    float totalWh = 0;

    /* Use a loop to calculate total energy consumption */
    for (int i = 0; i < count; i++) {
        float applianceWh = arr[i].watt * arr[i].quantity * arr[i].hours;
        totalWh += applianceWh;

        printf("%s: %dW × %d units × %.2f hours = %.2f Wh\n",
               arr[i].name, arr[i].watt,
               arr[i].quantity, arr[i].hours, applianceWh);
    }

    /* Print total daily energy consumption */
    printf("\nTotal daily energy usage: %.2f Wh\n", totalWh);

    return totalWh;
}

/* Convert watt-hours (Wh) to kilowatt-hours (kWh) */
float convertWhToKWh(float totalWh)
{
    float totalKWh = totalWh / 1000.0;
    printf("Total energy in kWh: %.2f kWh\n", totalKWh);
    return totalKWh;
}

/* Calculate recommended inverter size */
float calculateInverterFromWh(float totalWh)
{
    /* Calculate average hourly load */
    float avgLoad = totalWh / 24.0;

    /* Add 25% extra for surge capacity */
    float inverterVA = avgLoad * 1.25;

    printf("Recommended inverter size: %.0f VA\n", inverterVA);
    return inverterVA;
}

/* Calculate required number of solar panels */
int calculateSolarPanels(float totalWh)
{
    float peakSunHours = 4.5;   /* Teacher-provided peak sun hours */

    /* Calculate required PV array size */
    float pvSize = (totalWh / peakSunHours) * 1.3;

    /* Calculate number of panels (rounded up) */
    int numPanels = (int)(pvSize / 585 + 0.999);

    printf("Recommended solar panels: %d × 585W\n", numPanels);
    return numPanels;
}

/* Calculate battery bank size */
int calculateBatteryBank(float totalWh)
{
    const int batteryVoltage = 12;    /* Battery voltage */
    const int singleBatteryAh = 200;  /* Battery capacity */
    const float DoD = 0.5;            /* Depth of Discharge */

    /* Calculate required battery capacity in Ah */
    float requiredAh = (totalWh / batteryVoltage) / DoD;

    /* Calculate number of batteries (rounded up) */
    int numBatteries = (int)ceil(requiredAh / singleBatteryAh);

    printf("Recommended battery bank: %d × %dAh %dV\n",
           numBatteries, singleBatteryAh, batteryVoltage);

    return numBatteries;
}

int main(void)
{
    FILE *fp;
    int count = 0;
    char line[100];

    float totalWh, totalKWh, inverterVA;
    int panelCount, batteryCount;

    /* Open file to count number of appliances */
    fp = fopen("appliances.txt", "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    /* Use a while loop to count appliances */
    while (fgets(line, sizeof(line), fp) != NULL) {
        count++;
    }
    fclose(fp);

    /* Create a structure array using malloc */
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
    batteryCount = calculateBatteryBank(totalWh);

    /* Free allocated memory */
    free(list);

    return 0;
}
