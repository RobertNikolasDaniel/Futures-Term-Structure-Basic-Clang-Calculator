# Futures Term Structure: Market vs. Fair Value

**A C++ quantitative research project by Robert's Research**

**Author:** Robert Nikolas Daniel  
**GitHub:** RobertNikolasDaniel  
**Development Method:** Microsoft PowerShell and Windows Notepad  
**Code and Debugging:** 100% human-written and human-debugged. No generative AI was used to write or debug the code.

---

## 1. Project Overview

This C++ calculator examines the relationship between market-traded futures prices, underlying spot prices, financing conditions, and theoretical fair value.

The program accepts market prices for an underlying asset and two futures contracts, along with the underlying asset's yield, the time remaining until each contract's delivery, and a monthly term structure of annualized SOFR inputs.

Using these inputs, the calculator estimates financing carry, calculates theoretical futures values, measures market-traded basis relative to spot, and compares the near and far futures contracts through a calendar spread.

The central research question is:

**How does the observed futures term structure compare with a theoretical term structure derived from the underlying asset's yield and the financing cost implied by the SOFR curve?**

The objective is to establish a simple, transparent framework for examining futures pricing relative to a carry-based benchmark.

Rather than treating the observed futures price as an isolated market quote, the program connects it to the underlying asset, financing assumptions, and time to delivery.

## 2. Research Objectives

The calculator is designed to examine three related relationships.

### A. Futures versus Spot

The first comparison measures the futures contract's market price relative to the underlying spot price.

The program calculates a market-traded basis and compares it with an estimated fair value derived from financing carry.

This provides a framework for investigating whether the observed futures price is relatively rich or cheap against the model's assumptions.

### B. Financing Carry and Fair Value

The second comparison estimates the financing differential between the underlying asset's yield and the average SOFR input over the relevant delivery horizon.

The resulting carry estimate is used to discount the spot price exponentially, producing a theoretical futures value.

This allows the user to examine how different funding assumptions and delivery horizons affect the model's estimate of fair value.

### C. Near-versus-Far Calendar Spreads

The third comparison examines the difference between the near and far futures contracts.

The program calculates both the observed calendar spread and the difference between the two model-generated fair values.

Comparing these values provides a way to investigate whether the market's calendar spread differs from the spread implied by the model.

The broader objective is to study the relationship between funding conditions, time to delivery, and the shape of the futures term structure.

---

## 3. Mathematical Framework

### 3.1 Market-Traded Basis

The calculator measures the percentage difference between a futures price and the underlying spot price.

The implemented formula is:

\[
B=\frac{F-S}{S}
\]

Where:

- \(B\) = market-traded basis as a decimal.
- \(F\) = futures market price.
- \(S\) = underlying spot price.

The program converts this decimal into basis points:

\[
B_{\text{bps}}=\frac{F-S}{S}\times 10{,}000
\]

A positive basis indicates that the futures price is above spot. A negative basis indicates that the futures price is below spot.

This calculation is performed separately for the near and far futures contracts.

**Purpose:** To establish a normalized measure of the observed futures-to-spot relationship that can be compared across contracts and market conditions.

### 3.2 Funding Spread

The calculator estimates the cumulative financing differential between the underlying asset's annualized yield and the average annualized SOFR input.

The implemented formula is:

\[
C=(y-r)\times\frac{D}{365}
\]

Where:

- \(C\) = time-adjusted funding spread in basis points, assuming the input rates are entered in basis points.
- \(y\) = underlying asset yield or dividend input.
- \(r\) = average SOFR input.
- \(D\) = days remaining until delivery.

The calculation can also be written as:

\[
C=y\frac{D}{365}-r\frac{D}{365}
\]

This represents the difference between the simple, time-adjusted asset yield and financing cost over the specified horizon.

The program calculates the funding spread independently for the near and far futures contracts.

**Purpose:** To estimate how the relationship between asset yield and financing cost changes across delivery horizons.

### 3.3 Average SOFR Term Structure

Rather than accepting only one SOFR observation for each futures contract, the program asks the user to enter annualized SOFR inputs for individual months.

These observations are stored in separate vectors for the near and far delivery horizons.

For the near contract, the program calculates:

\[
\bar r_{\text{near}}=
\frac{1}{N_{\text{near}}}
\sum_{i=1}^{N_{\text{near}}}r_i
\]

For the far contract:

\[
\bar r_{\text{far}}=
\frac{1}{N_{\text{far}}}
\sum_{i=1}^{N_{\text{far}}}r_i
\]

Where:

- \(r_i\) = annualized SOFR input for month \(i\).
- \(N\) = number of monthly observations entered.

The resulting averages become the SOFR inputs used in the funding-spread calculations.

This allows the model to use different average financing assumptions for different delivery horizons.

**Important modeling assumption:** The program uses an arithmetic average of the entered monthly rates. It does not calculate a compounded SOFR path or derive a daily financing curve from actual market observations.

### 3.4 Exponential Fair Value

The calculator converts the time-adjusted funding spread from basis points into a decimal:

\[
c=\frac{C}{10{,}000}
\]

It then calculates the exponential discount factor:

\[
DF=e^{-c}
\]

The theoretical futures value is calculated as:

\[
F_{\text{fair}}=S e^{-c}
\]

Where:

- \(F_{\text{fair}}\) = model-generated futures fair value.
- \(S\) = underlying spot price.
- \(c\) = time-adjusted funding spread expressed as a decimal.
- \(e\) = the base of the natural logarithm.

The program implements this calculation using the C++ exponential function, `std::exp()`.

The same calculation is applied to both the near and far contracts using their respective funding-spread estimates.

**Purpose:** To translate the estimated financing differential into a theoretical price adjustment relative to spot.

This exponential relationship provides a continuous discounting approximation. It should not be interpreted as a complete pricing model for every futures contract or underlying asset.

### 3.5 Futures Market Price Versus Fair Value

After calculating theoretical fair value, the program compares it with the observed market price.

The point difference is:

\[
\Delta F=F_{\text{market}}-F_{\text{fair}}
\]

A positive difference means the market price is above the model's fair value.

A negative difference means the market price is below the model's fair value.

A zero difference means the market price matches the model's estimate.

The program reports these comparisons separately for the near and far futures contracts.

**Purpose:** To identify the direction and magnitude of the deviation between the observed market price and the model-generated benchmark.

### 3.6 Calendar Spread Analysis

The calculator examines the relationship between the far and near futures contracts.

The observed calendar spread is:

\[
CS_{\text{market}}=F_{\text{far}}-F_{\text{near}}
\]

The model-generated calendar spread is:

\[
CS_{\text{fair}}=
F_{\text{far,fair}}-F_{\text{near,fair}}
\]

The difference between these two spreads is:

\[
\Delta CS=
CS_{\text{market}}-CS_{\text{fair}}
\]

Equivalently:

\[
\Delta CS=
(F_{\text{far}}-F_{\text{near}})
-
(F_{\text{far,fair}}-F_{\text{near,fair}})
\]

A positive result indicates that the observed calendar spread is wider than the model-generated spread.

A negative result indicates that the observed calendar spread is narrower than the model-generated spread.

A zero result indicates that both spreads are equal.

**Purpose:** To compare the market's pricing of the relative value between two delivery horizons against a benchmark derived from the program's carry assumptions.

This is the central comparative output of the project: the difference between the observed futures term structure and the term structure implied by the model.

---

## 4. Program Structure

The project is organized into individual mathematical functions and a main routine responsible for user inputs, calculations, and output.

### `marketbasis()`

Calculates the normalized difference between a futures price and the underlying spot price.

### `fundingspread()`

Calculates the time-adjusted difference between the underlying asset's yield and the average SOFR input.

### `fairvalue()`

Calculates the exponential discount factor using the funding-spread input.

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

The program uses C++ standard-library components, including:

- `<iostream>` for console input and output.
- `<cmath>` for mathematical operations, including the exponential function.
- `<vector>` for storing monthly SOFR observations.

The vector-based approach allows the user to supply a variable number of monthly rate observations for each delivery horizon.

---

## 5. Development Philosophy

This project was written manually using **Windows Notepad and PowerShell**.

No generative AI was used to generate the code or debug it.

The implementation, debugging process, and development decisions were carried out manually by the author.

The objective was to build a working quantitative research tool through direct engagement with the programming language, the mathematical relationships, and the development process itself.

PowerShell provided the command-line environment, while Notepad served as the text editor.

This workflow deliberately avoids dependence on AI-generated implementations and AI-assisted debugging.

The project represents an effort to connect quantitative financial research with hands-on software development, using a straightforward environment in which the author maintains direct responsibility for the code.

**The emphasis is on understanding the mathematics, implementing it manually, and taking responsibility for the resulting program.**

---

## 6. Scope and Limitations

The calculator is a research prototype, not a production pricing or trading system.

Its outputs depend on the inputs supplied by the user and the assumptions embedded in its formulas.

In particular:

- **Rate units:** The formulas assume yields and SOFR inputs are entered in basis points. Entering decimal rates or percentage values without conversion will produce incorrect results.
- **Average financing rates:** Monthly SOFR observations are averaged arithmetically rather than compounded or weighted by the number of days in each observation period.
- **Day-count convention:** The funding-spread calculation uses a 365-day denominator.
- **Pricing approximation:** The exponential fair-value calculation is a simplified carry-based benchmark. It does not explicitly model every contractual or market-specific feature of a futures contract.
- **Asset yield:** The program treats the supplied yield or dividend input as a single annualized rate. It does not independently model the timing of dividend payments or other cash flows.
- **Futures conventions:** The model does not explicitly account for variation-margin cash flows, daily settlement, delivery options, contract-specific conversion factors, or other instrument-specific pricing mechanics.
- **Input validation:** The program relies on user-supplied inputs and is not designed to handle every invalid or economically inconsistent input.
- **Interpretation:** A deviation from model fair value is not, by itself, evidence of a profitable arbitrage opportunity. Market frictions, model assumptions, and instrument-specific characteristics may explain observed differences.

These limitations define the current scope of the project and provide context for interpreting its outputs.

---

## 7. Intended Applications

The project is intended for quantitative research and educational analysis involving:

- Futures-versus-spot basis.
- Financing carry and theoretical price relationships.
- SOFR term-structure assumptions.
- Near-versus-far futures calendar spreads.
- Market pricing relative to a simplified fair-value benchmark.
- The transmission of financing assumptions into futures term structure.

It provides a transparent framework for testing how different market prices, yields, funding assumptions, and delivery horizons affect a carry-based valuation.

The calculator is not intended to produce trading recommendations or guarantee a particular market outcome.

---

## 8. Author

**Robert Nikolas Daniel**  
Independent Quantitative Researcher and Trader

Research interests include fixed-income markets, financing conditions, futures pricing, term structure, and the relationship between funding costs and relative-value opportunities.

GitHub: `RobertNikolasDaniel`

---

## 9. Summary

This project combines a simplified carry-based pricing framework with a manually developed C++ implementation.

It calculates the observed futures basis, estimates financing carry from an underlying yield and average SOFR inputs, derives theoretical futures values using exponential discounting, and compares the observed near-versus-far calendar spread with the spread implied by those theoretical values.

The core research objective is straightforward:

**Measure how the market-traded futures term structure differs from a theoretical benchmark derived from financing carry.**

The code was written and debugged manually in PowerShell and Notepad, without generative AI code generation or AI debugging.

The result is an independently developed quantitative research project focused on transparent mathematics, direct implementation, and the analysis of futures pricing relative to funding assumptions.
