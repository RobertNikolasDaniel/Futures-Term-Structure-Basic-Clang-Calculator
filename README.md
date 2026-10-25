# Futures Term Structure: Market vs. Fair Value

**An Independent Quantitative Research Project by Robert Nikolas Daniel**

**Language:** C++  
**Development Environment:** Windows PowerShell and Notepad  
**Code Generation and Debugging:** 100% human-written and human-debugged. No generative AI was used.

---

## 1. Project Overview

This C++ calculator examines the relationship between futures market prices, underlying spot prices, financing conditions, and theoretical fair value.

The program accepts market prices for an underlying asset and two futures contracts, the underlying asset's annualized yield, the time remaining until each contract's delivery, and monthly annualized SOFR inputs.

Using these inputs, the calculator estimates financing carry, calculates theoretical futures values, measures market-traded basis relative to spot, and compares the near and far futures contracts through a calendar spread.

The central research question is:

**How does the observed futures term structure compare with a theoretical term structure derived from the underlying asset's yield and financing costs?**

Rather than treating futures prices as isolated market observations, this project examines how the relationship between asset yield, financing assumptions, and time to delivery influences theoretical pricing.

The goal is to provide a transparent framework for comparing observed market prices against a simplified carry-based benchmark.

## 2. Research Objectives

The calculator examines three primary relationships.

### A. Futures Versus Spot

The program calculates the market-traded basis between each futures contract and the underlying spot price.

It then compares the observed futures price with a theoretical fair value derived from the model's financing assumptions.

This provides a framework for investigating whether a futures contract trades relatively rich or cheap against the model's benchmark.

### B. Financing Carry and Fair Value

The program estimates the time-adjusted difference between the underlying asset's yield and the average SOFR input.

This funding spread is used to calculate an exponential discount factor, which is applied to the spot price to estimate theoretical futures fair value.

The resulting values allow the user to examine how different funding assumptions and delivery horizons affect the model's theoretical pricing.

### C. Near-versus-Far Calendar Spreads

The program compares the observed price difference between near and far futures contracts with the difference between their theoretical fair values.

This comparison provides a way to investigate whether the market's calendar spread differs from the spread implied by the model's carry assumptions.

The broader objective is to study the relationship between financing conditions, time to delivery, and the shape of the futures term structure.

---

## 3. Mathematical Framework

### 3.1 Market-Traded Basis

The calculator measures the normalized difference between a futures market price and the underlying spot price.

**Formula:**

`Market Basis (bps) = ((Futures Price - Spot Price) / Spot Price) * 10,000`

Where:

- **Futures Price:** The observed market price of the futures contract.
- **Spot Price:** The current market price of the underlying asset.
- **10,000:** Converts the decimal result into basis points.

A positive basis indicates that the futures price is above spot. A negative basis indicates that the futures price is below spot.

The calculation is performed independently for the near and far futures contracts.

**Purpose:** Establishes a normalized measure of the observed futures-to-spot relationship.

### 3.2 Funding Spread

The calculator estimates the time-adjusted difference between the underlying asset's yield and the average SOFR input.

**Formula:**

`Funding Spread (bps) = (Asset Yield - Average SOFR) * (Days to Delivery / 365)`

Where:

- **Asset Yield:** The annualized yield or dividend input, in basis points.
- **Average SOFR:** The arithmetic average of the monthly SOFR inputs, in basis points.
- **Days to Delivery:** The number of days remaining until the futures contract's delivery date.
- **365:** The day-count denominator used by the model.

This calculation estimates the cumulative yield-minus-financing differential over the specified delivery horizon.

The program calculates the funding spread separately for the near and far futures contracts.

**Purpose:** Measures how the estimated financing differential changes with the underlying yield, SOFR assumptions, and time to delivery.

### 3.3 Average SOFR Term Structure

The program collects monthly annualized SOFR inputs and stores them in separate vectors for the near and far futures contracts.

For each contract, the arithmetic average is calculated by summing the monthly observations and dividing by the number of observations entered.

**Formula:**

`Average SOFR = Sum of Monthly SOFR Inputs / Number of Monthly Inputs`

The resulting averages are used as the SOFR inputs in the respective funding-spread calculations.

This allows the near and far contracts to have different average financing assumptions.

**Important modeling assumption:** The program uses an arithmetic average of the entered monthly rates. It does not calculate compounded SOFR or weight observations according to the number of days in each month.

**Purpose:** Provides a simplified representation of the funding-rate assumptions associated with each delivery horizon.

### 3.4 Exponential Fair Value

The calculator converts the time-adjusted funding spread from basis points into a decimal before applying exponential discounting.

**Step 1: Convert the funding spread into a decimal.**

`Funding Spread (Decimal) = Funding Spread (bps) / 10,000`

**Step 2: Calculate the exponential discount factor.**

`Discount Factor = exp(-Funding Spread (Decimal))`

**Step 3: Calculate theoretical futures fair value.**

`Fair Value = Spot Price * exp(-Funding Spread (Decimal))`

The exponential function is implemented using the C++ standard-library function `std::exp()`.

The program calculates fair value independently for the near and far futures contracts using their respective funding-spread estimates.

**Purpose:** Translates the estimated financing differential into a theoretical price adjustment relative to spot.

This is a simplified exponential carry approximation, not a complete pricing model for every futures contract or underlying asset.

### 3.5 Market Price Versus Fair Value

After calculating theoretical fair value, the program compares it with the observed futures market price.

**Formula:**

`Price Deviation = Market Futures Price - Fair Value`

Interpretation:

- **Positive deviation:** The market price is above the model's fair value.
- **Negative deviation:** The market price is below the model's fair value.
- **Zero deviation:** The market price equals the model's fair value.

The program reports these comparisons separately for the near and far contracts.

**Purpose:** Measures the difference between observed market pricing and the model's theoretical benchmark.

### 3.6 Calendar Spread Analysis

The calculator compares the market-traded calendar spread with the calendar spread implied by its fair-value calculations.

**Market calendar spread:**

`Market Calendar Spread = Far Futures Price - Near Futures Price`

**Fair-value calendar spread:**

`Fair-Value Calendar Spread = Far Futures Fair Value - Near Futures Fair Value`

**Calendar spread deviation:**

`Calendar Spread Deviation = Market Calendar Spread - Fair-Value Calendar Spread`

Interpretation:

- **Positive deviation:** The market calendar spread is wider than the model's fair-value spread.
- **Negative deviation:** The market calendar spread is narrower than the model's fair-value spread.
- **Zero deviation:** The market calendar spread equals the model's fair-value spread.

The program also calculates the difference between the near and far contracts' time-adjusted funding spreads.

**Funding spread differential:**

`Funding Spread Differential = Far Funding Spread - Near Funding Spread`

This provides an additional comparison of the estimated financing differential across delivery horizons.

**Purpose:** Examines whether the relative pricing between two futures delivery horizons differs from the relationship implied by the model's carry assumptions.

This is the central comparative output of the project: the difference between the observed futures term structure and the term structure implied by the model.

---

## 4. Program Structure

The project is organized around three mathematical functions and a main routine responsible for inputs, calculations, and output.

### `marketbasis()`

Calculates the normalized difference between a futures price and the underlying spot price.

### `fundingspread()`

Calculates the time-adjusted difference between the underlying asset's yield and the average SOFR input.

### `fairvalue()`

Calculates the exponential discount factor from the funding-spread input.

### `main()`

Coordinates the complete calculation process.

The main routine:

1. Collects the underlying spot price and near and far futures prices.
2. Collects the underlying asset's yield or dividend input.
3. Accepts monthly SOFR observations for each delivery horizon.
4. Calculates average SOFR for the near and far contracts.
5. Collects the days remaining until each delivery date.
6. Calculates the funding spread and theoretical fair value for each contract.
7. Compares the observed futures prices with their theoretical values.
8. Calculates the market-traded basis for each contract.
9. Calculates the observed and theoretical calendar spreads.
10. Reports the difference between the observed and theoretical calendar spreads.

### C++ Standard Libraries

The program uses the following standard-library components:

- `<iostream>` for console input and output.
- `<cmath>` for mathematical operations, including exponential calculations.
- `<vector>` for storing monthly SOFR observations.

The vector-based approach allows the user to enter a variable number of monthly rate observations for each delivery horizon.

---

## 5. Development Philosophy

This project was written manually using **Windows PowerShell and Windows Notepad**.

**No generative AI was used to write the code or debug it.**

The implementation, debugging process, and development decisions were carried out manually by the author.

PowerShell provided the command-line environment, while Notepad served as the text editor.

This workflow emphasizes direct engagement with the programming language, the mathematical relationships, and the debugging process rather than reliance on AI-generated implementations.

The author takes direct responsibility for the code and its development.

The objective is to build quantitative research tools through a combination of financial theory, mathematical reasoning, and hands-on programming.

**The emphasis is on understanding the mathematics, implementing it manually, and taking responsibility for the resulting program.**

---

## 6. Scope and Limitations

This project is a quantitative research prototype, not a production pricing or trading system.

Its outputs depend on the inputs supplied by the user and the assumptions embedded in the model.

### Rate Units

The formulas assume that the asset yield and SOFR inputs are entered in basis points. Entering decimal rates or percentage values without conversion will produce incorrect results.

### Average Financing Rates

The program calculates an arithmetic average of the monthly SOFR observations.

It does not calculate a compounded SOFR path or weight the observations by the number of days in each period.

### Day-Count Convention

The funding-spread calculation uses a 365-day denominator.

### Simplified Pricing Model

The exponential fair-value calculation is a simplified carry-based approximation.

It does not explicitly model every contractual or market-specific feature of futures pricing, including daily variation-margin cash flows, delivery options, or contract-specific pricing conventions.

### Underlying Asset Yield

The model treats the supplied yield or dividend as a single annualized input.

It does not independently model the timing of dividend payments or other underlying cash flows.

### Input Validation

The program relies on user-supplied inputs and does not validate every possible invalid or economically inconsistent value.

### Interpretation of Results

A deviation from theoretical fair value does not, by itself, establish a profitable arbitrage opportunity.

Observed market prices can differ from a simplified model because of market frictions, liquidity, contract specifications, financing conditions, and assumptions that are not explicitly represented in the calculations.

These limitations define the current scope of the project and provide context for interpreting its outputs.

---

## 7. Intended Applications

The calculator is intended for quantitative research and educational analysis involving:

- Futures-versus-spot basis.
- Financing carry and theoretical price relationships.
- SOFR term-structure assumptions.
- Near-versus-far futures calendar spreads.
- Market pricing relative to a simplified fair-value benchmark.
- The relationship between financing conditions and futures term structure.
- Comparative analysis of market-observed and model-generated pricing relationships.

The program provides a transparent framework for examining how different market prices, asset yields, funding assumptions, and delivery horizons affect a carry-based valuation.

It is not intended to generate trading recommendations or guarantee particular market outcomes.

---

## 8. Author

**Robert Nikolas Daniel**  
Independent Quantitative Researcher and Trader

Research interests include fixed-income markets, financing conditions, futures pricing, term structure, and the relationship between funding costs and relative-value opportunities.

**GitHub:** RobertNikolasDaniel

---

## 9. Summary

Futures Term Structure: Market vs. Fair Value is an independently developed C++ quantitative research project examining the relationship between market prices, financing carry, and theoretical futures valuations.

The calculator:

- Measures the market-traded futures-to-spot basis.
- Estimates time-adjusted financing spreads using asset yield and average SOFR inputs.
- Calculates theoretical futures fair values using exponential discounting.
- Compares market prices against theoretical values.
- Evaluates the observed calendar spread against the model-generated fair-value spread.

The central research objective is straightforward:

**Measure how the market-traded futures term structure differs from a theoretical benchmark derived from financing carry.**

The code was written and debugged manually using PowerShell and Notepad, without generative AI code generation or AI debugging.

The result is an independently developed quantitative research tool focused on transparent mathematics, direct implementation, and the analysis of futures pricing relative to financing assumptions.
