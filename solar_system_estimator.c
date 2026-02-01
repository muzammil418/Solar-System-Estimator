#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ===== FIXED PRICES (PKR) ===== */
#define PANEL_PRICE     20000   // price per 585W panel
#define INVERTER_PRICE  62000  // inverter price
#define LA_PRICE        45000   // Lead Acid battery price
#define LI_PRICE        220000  // Lithium battery price

/* Store appliance data */
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
    if (!fp) {
        printf("Error opening file\n");
        exit(1);
    }

    for (int i = 0; i < count; i++) {
        fscanf(fp, " %[^,],%d", arr[i].name, &arr[i].watt);
    }
    fclose(fp);
}

/* User input */
void getUsageFromUser(struct Appliance *arr, int count)
{
    for (int i = 0; i < count; i++) {
        printf("\n%s (%dW)\n", arr[i].name, arr[i].watt);
        printf("Quantity: ");
        scanf("%d", &arr[i].quantity);
        printf("Hours per day: ");
        scanf("%f", &arr[i].hours);
    }
}

/* Energy calculation */
float calculateDailyEnergy(struct Appliance *arr, int count)
{
    float totalWh = 0;

    printf("\n%-25s %-6s %-6s %-8s %-10s\n",
           "Appliance", "Watt", "Qty", "Hours", "Wh");

    for (int i = 0; i < count; i++) {
        float wh = arr[i].watt * arr[i].quantity * arr[i].hours;
        totalWh += wh;

        printf("%-25s %-6d %-6d %-8.2f %-10.2f\n",
               arr[i].name, arr[i].watt,
               arr[i].quantity, arr[i].hours, wh);
    }

    printf("\nTotal Daily Energy: %.2f Wh\n", totalWh);
    return totalWh;
}

/* Inverter sizing (WATT based) */
float calculateInverter(float totalWh)
{
    float inverterWatt = totalWh * 1.25;
	int inverterKW = (int)ceil(inverterWatt / 1000.0);
    int inverterCost = inverterKW * INVERTER_PRICE;
	
    printf("Recommended Inverter: %.0f W\n", inverterWatt);
	printf("Inverter Selected: %d kW\n", inverterKW);
	
    return inverterKW;
}

/* Solar panels calculation */
int calculateSolarPanels(float totalWh)
{
    float peakSunHours = 4.5;
    int panelWatt = 585;

    float pvSize = (totalWh / peakSunHours) * 1.3;
    int panels = (int)ceil(pvSize / panelWatt);

    printf("Solar Panels Required: %d x %dW\n", panels, panelWatt);
    return panels;
}

/* Battery calculation (night load only) */
void calculateBattery(float nightWh,
                      int *laCount, int *liCount,
                      float *laCap, float *liCap)
{
    int V = 12, Ah = 200;

    *laCount = ceil((nightWh / V) / 0.5 / Ah);
    *liCount = ceil((nightWh / V) / 0.8 / Ah);

    *laCap = (*laCount * V * Ah) / 1000.0;
    *liCap = (*liCount * V * Ah) / 1000.0;
}

int main()
{
    FILE *fp;
    char line[100];
    int count = 0;

    /* Count appliances */
    fp = fopen("appliances.txt", "r");
    if (!fp) {
        printf("File not found\n");
        return 1;
    }
    while (fgets(line, sizeof(line), fp)) count++;
    fclose(fp);

    struct Appliance *list = malloc(count * sizeof(struct Appliance));
    if (!list) return 1;

    loadAppliances(list, count);
    getUsageFromUser(list, count);

    float totalWh = calculateDailyEnergy(list, count);

    int inverterKw = calculateInverter(totalWh);
    int panels = calculateSolarPanels(totalWh);

    /* Night load (14 hours) */
    float nightWh = totalWh * (14.0 / 24.0);

    int laCount, liCount;
    float laCap, liCap;
    calculateBattery(nightWh, &laCount, &liCount, &laCap, &liCap);

    /* Cost summary */
    printf("\n--- System Cost Summary ---\n");
	
	printf("Per Pannel Cost 20000\n");
    printf("Solar Panels Cost: PKR %d\n", panels * PANEL_PRICE);
	printf("Inverter per KW cost 62000\n");
    printf("Inverter Cost: PKR %d\n",  inverterKw * INVERTER_PRICE);

    printf("\nBattery Options:\n");
	printf("Lead Acide battery 200Ah 12V cost 45000\n");
    printf("Lead Acid: %d batteries | %.2f kWh | Cost: PKR %d | Life: 2000-3000 cycles\n",
           laCount, laCap, laCount * LA_PRICE);
	
	printf("Lithium battery 200Ah 12V cost 220000\n");
    printf("Lithium:   %d batteries | %.2f kWh | Cost: PKR %d | Life: 6000-10000 cycles\n",
           liCount, liCap, liCount * LI_PRICE);
	
    free(list);
    return 0;
}
