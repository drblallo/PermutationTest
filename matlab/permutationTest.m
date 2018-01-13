function successPercent = permutationTest()
    alpha = 0.05;
    numberOfPermutation = 1000; %M
    sampleSize = 100; %N
    cutPoint = 50; %m 
    numberOfIterations = 1000;
    
    successPercent = 0;
    successData = zeros(1, numberOfPermutation - 50);
    builtInTestSuccess = 0;
    
    
    for a = 50:numberOfPermutation
        successPercent = 0;
        for t = 1:numberOfIterations
            [sample, sample2] = generateSample(sampleSize);       
            successPercent = successPercent + runPermutationTest(sample, sample2, alpha, a);
            %builtInTestSuccess = builtInTestSuccess + mwtest(sample, cutPoint, sampleSize);
            %builtInTestSuccess = builtInTestSuccess + ttest(sample, cutPoint, sampleSize);

        end
        successData(a - 49) = successPercent/numberOfIterations;
        disp(a);
    end
     
    y = linspace(50, numberOfPermutation, numberOfPermutation - 49);
    disp(size(y));
    disp(successData);
    disp(size(successData));
    %disp(builtInTestSuccess/numberOfIterations);
    scatter(y, successData);
    successPercent = successPercent/numberOfIterations*100;
end

function [sample, sample2] = generateSample(sampleSize)
    sample = normrnd(0, 1, [sampleSize/2, 1]);
    sample2 = normrnd(0, 1, [sampleSize/2, 1]);
end

function success = ttest(sample, sample2)
    success = 1-ttest2(sample, sample2);   
end

function success = mwtest(sample, sample2)
    [p, h] = ranksum(sample, sample2);
    success = 1-h;
end

function statistic = evaluateStatistic(sample, sample2)
    
    a = 0;
    b = 0;
    
    %for t = 1:cutPoint
    %    a = a + sample(t);
    %end
    
    
    %a = a / cutPoint;
    
    a = mean(sample);
    
    %for t = cutPoint+1:lenght
    %    b = b + sample(t);
    %end
   

    %b = b / (lenght - cutPoint);
    
    b = mean(sample);
    
    statistic = a - b;
    if (statistic < 0)
        statistic = statistic * -1;
    end
    
end

function statistic = evaluateStatisticMW(sample, sample2)
   
    [p, h, stat] = ranksum(sample, sample2);
    statistic = stat.zval;
    %disp(statistic);
    
    if (statistic < 0)
        statistic = statistic * -1;
    end
end

function h0Accepted = runPermutationTest(sample, sample2, alpha, numberOfP)

    statistics = zeros(1, numberOfP);    
    sampleStatistic = evaluateStatistic(sample, sample2);
    equalOccurences = 0;
    higherOccurences = 0;
    lenght = size(sample);
    lenght = lenght(1);
    lenght2 = size(sample2);
    lenght2 = lenght2(1);
    %disp(alpha);
    %disp(cutPoint);
    %disp(numberOfP);
    %disp(lenght);
    %disp('-');
    
    
    for t = 1:numberOfP
        statistics(t) = evaluateStatistic(sample(randperm(lenght)), sample2(randperm(lenght2))); 
        
        if (statistics(t) == sampleStatistic)
            equalOccurences = equalOccurences + 1;
        end
        
        if (statistics(t) > sampleStatistic)
            higherOccurences = higherOccurences + 1;
        end
    end
    
    statistics = sort(statistics);
    %disp(statistics);
    
    k = numberOfP - floor(alpha*numberOfP);
    %disp(statistics(k));
    %disp(sampleStatistic);
    if (statistics(k) > sampleStatistic)
        h0Accepted = 1;
       % disp(sampleStatistic);
       % disp(statistics(k));
        return
    end
    
    if (statistics(k) < sampleStatistic)
        h0Accepted = 0;
        return
    end
    
    treshold = (alpha*numberOfP) - higherOccurences;
    treshold = treshold / equalOccurences;
    
    if (rand() > treshold)
        h0Accepted = 0;
        return;
    end
        
    h0Accepted = 1;
    
end