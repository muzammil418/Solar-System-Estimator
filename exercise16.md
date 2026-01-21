Worksheet 16
Date: 20th of Janurary, 2026
Points: 10

Note:
1. Do not write code that you don't understand or would not be able to explain yourself
2. Create a new GitHub repository called "Solar System Estimator" and push this file and the solution of this exercise there!
3. Use any and all tools/techniques necessary (including AI) to answer questions, write or understand code, or ask for clarifications
4. In case the requirements are unclear, ask an AI or an expert on this matter. Gather information and decide on the solution on your own.
5. You can change the goals of this exercise, if you make them better. In such a case, you need to have a justification for that change.
6. You do not need to write answers for "Stop-and-Learn Checkpoints", but you need to know them enough to able to explain them (verbally - on your own) and implement them in code.

===========================================================================================

# **Solar Apprentice: Designing Your First Solar Energy Tool**

## **Chapter 1: Welcome to the Solar Team**

You've just joined SolarWorks, a growing solar energy company. Your first assignment as a junior developer is to create a tool that helps customers understand what size solar system their home needs.

The senior engineer hands you a specification sheet:

> "We need a console program that estimates solar system sizes based on a household's appliances. Customers provide their appliance list and usage patterns, and our tool calculates their energy needs, inverter size, solar panel requirements, and battery bank."

Your code will directly help people make informed decisions about solar systems.

### **Stop-and-Learn Checkpoint**

What is a Solar Cell?

What is a Solar Panel?

---

## **Chapter 2: Understanding the Language of Energy**

Before we design anything, we need to speak the language of energy. Let's start simple:

**Watts (W)** are like **speed**—how fast an appliance uses energy. A 100W bulb uses energy faster than a 10W LED.

**Watt-hours (Wh)** are like **distance traveled**—the total energy used. If that 100W bulb runs for 3 hours: 100 × 3 = 300Wh.

### **Stop-and-Learn Checkpoint**
**Pause and think:** Walk around your home. What appliances are running right now? Which ones use energy fast (high watts)? Which ones run for many hours? Make mental notes—we'll use this soon.

How much power does your laptop consumes in 1 hour, 5 hours, and 10 hours of contineous use?

---

## **Chapter 3: The Apprentice's Toolbox**

Every craftsperson needs tools. For our solar planning tool, we need:

1. **A database** of common appliances and their wattages
2. **A way to remember** each appliance's details
3. **A way to ask** the homeowner about their usage

Let's start with the simplest tool first: our appliance database.

In your project folder, create a file named `appliances.txt` with this data:

```
Ceiling Fan,70
LED Tube Light,20
LED Bulb,10
Refrigerator,150
Television,80
Laptop,60
Desktop Computer,150
Wi-Fi Router,10
Mobile Charger,5
Water Pump,750
Air Cooler,200
Air Conditioner,1500
Mixer Grinder,500
Electric Kettle,1500
Microwave Oven,1000
Iron,1200
```

This file should be placed in the same directory as your C program. Each appliance name and wattage is separated by a comma—a simple format you can read with `fscanf()` or `fgets()` and `sscanf()`.

### **Stop-and-Learn Checkpoint**
Are the given watts correct in the list?

Who can you ask to verify?

Can you think of other appliances that are not present in this list?

---

## **Chapter 4: Building Your Memory System**

Our program needs to remember each appliance's name and wattage. In real life, you'd write it in a notebook. In C, we use a **structure**—a way to group related information together.

```c
struct Appliance {
    char name[50];
    int wattage;
};
```

But wait—we don't know how many appliances are in the file! It could be 10 today, 20 tomorrow. We need **dynamic memory allocation**—like asking for exactly how many notebook pages we need, no more, no less.

### **Stop-and-Learn Checkpoint**
🤔 **Before coding:** Open `appliances.txt` in any text editor. Count how many appliances are there. What happens if someone adds more appliances later? How would your program handle that without you changing the code?

Dynamic memory allocation can be done through the "malloc" function. "malloc" is a short name for "Memory Allocation".

How does the malloc function work?

What does the malloc function return, if it's successful?

Can you allocate a simple array of integers with malloc? ... try!

Can you allocate the correct ammount of memory required for a structure with it?

Can you allocate the correct ammount of memory required for an array of structures with it?

What's the advantage of using "malloc" over allocating a large array in a program (of size 1000 for example)?

---

## **Chapter 5: The Interview—Learning About the Home**

Now we become solar detectives. We need to ask the homeowner (the user) about their energy habits.

For **each appliance**, we need to know:
- How many do you have? (Quantity)
- How many hours per day do you use them?

Think of this like a doctor asking questions before making a diagnosis.

Here's where **arrays** become useful—they let us keep all the answers organized, like a spreadsheet in memory.

### **Coding Moment**
Let's sketch our main approach:

```c
#include <stdio.h>
#include <stdlib.h>

struct Appliance {
    char name[50];
    int wattage;
    int quantity;
    float hours;
};

int main() {
    printf("=== Solar System Estimator ===\n");
    printf("Welcome! We're going to design a solar system for your home.\n\n");
    
    // First, we'll load appliances from the file
    // Then, for each appliance, ask: How many? How long?
    // Finally, we'll do our calculations
    
    return 0;
}
```

### **Stop-and-Learn Checkpoint**
Is it better to let the user write "fan" as input, or is it better to show them a list of appliances with a numbering and let the user select a number instead of writing it's full name?

---

## **Chapter 6: The Calculations—From Usage to Design**

### **Part 1: Daily Energy Consumption**
This is simple math but important:
```
Energy per appliance = wattage × quantity × hours
Total daily energy = sum of all appliances' energy
```

### **Stop-and-Learn Checkpoint**
What do these formulas mean?

Why do we multiply when calculating "Energy per appliance"?

### **Part 2: The Inverter—The Heart of the System**
The **inverter** converts battery power (DC) to home power (AC). It's like a translator between two languages.

**Rule of thumb:** Inverter size should be 25% more than your maximum simultaneous load. Why? Because appliances sometimes need a power surge when starting up.


### **Stop-and-Learn Checkpoint**
🤔 **Real-world thinking:** Do ALL your appliances run at the same time? Probably not. But which ones MIGHT run together? A refrigerator runs periodically 24 hours. Lights come on at night. What combinations should we plan for? Think about your own home's patterns.

Learn more about "Inverters" from youtube or AI or elsewhere.

If the wattage requirement of a user is 10kW, how would you calculate 25% more to estimate the correct size of Inverter?

### **Part 3: Solar Panels—Catching Sunlight**
Solar panels are rated in Watts. But they only produce power when the sun shines!

**Peak sun hours** = equivalent hours of full sunlight per day. In South Asia: typically 4-5 hours.

```
PV Array Size = (Daily Energy ÷ Peak Sun Hours) × 1.3
```
The 1.3 accounts for system losses—nothing in engineering is 100% efficient!

### **Stop-and-Learn Checkpoint**

What does "1.3" does in the above formula?

How does it account for system losses?

What are the causes of system losses in this system?

### **Part 4: Batteries—Storing Sunlight for Night**
Here's where **Depth of Discharge (DoD)** becomes crucial.

Imagine your phone battery: if you use it from 100% to 0% every day, it wears out fast. If you only use it from 100% to 50%, it lasts years.

**DoD** = how much of the battery's capacity we actually use. For lead-acid batteries (common in solar), we use only 50% to make them last 3-5 years.

```
Battery Capacity (Ah) = (Daily Energy ÷ Battery Voltage) ÷ DoD
```
We'll use 12V batteries—the standard for home systems.

---

## **Chapter 7: Putting It All Together**

Your final program should:
1. Load appliances from the file
2. Interview the user about their usage
3. Calculate and print a complete system design

Example output:
```
=== Solar System Design Complete ===

Your home uses 4500 Wh daily.

Recommended System:
1. Inverter: 1500 VA (to handle power surges)
2. Solar Panels: 1500 W (assuming 4 peak sun hours)
3. Batteries: 4 × 200Ah 12V batteries (2 days backup, 50% DoD)

Remember: Depth of Discharge means using only 50% of battery 
capacity to make your batteries last 4-5 years instead of 1-2.
```

### **Stop-and-Learn Checkpoint**

Is the recommended system correct? If not, how would you correctly estimate it?

---

## **Chapter 8: Your Mission, Should You Choose to Accept It**

You now have all the pieces. Your journey as a solar apprentice involves:

**Phase 1:** Build the foundation—read the appliance file into an array of structures.

**Phase 2:** Conduct the interview—ask about quantities and hours for each appliance.

**Phase 3:** Implement the calculations—energy, inverter, panels, batteries.

**Phase 4:** Present the results clearly, with explanations in plain language.

**Bonus Challenge:** What if the user has an appliance not in your database? Let them add it temporarily! (Hint: This might involve resizing your dynamic array.)

---

## **Final Guidance**

Remember: You're not just writing code. You're:
- Solving a real problem your community faces
- Learning engineering thinking
- Creating something that could actually help someone

When you get stuck (every apprentice does!), ask:
1. What would I do with pen and paper?
2. How can I break this into smaller steps?
3. What data do I need to remember at each step?

The solar panels on your future roof will convert sunlight to electricity. Your program will convert knowledge into a working plan. Both are acts of creation.

