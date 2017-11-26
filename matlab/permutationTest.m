function successPercent = permutationTest()
    alpha = 0.05;
    numberOfPermutation = 100; %M
    sampleSize = 100; %N
    cutPoint = 50; %m 
    numberOfIterations = 100;
    
    successPercent = 0;
    builtInTestSuccess = 0;
    
    for t = 1:numberOfIterations
        sample = generateSample(sampleSize);       
        successPercent = successPercent + runPermutationTest(sample, alpha, cutPoint, numberOfPermutation);
        builtInTestSuccess = builtInTestSuccess + mwtest(sample, cutPoint, sampleSize);
        %builtInTestSuccess = builtInTestSuccess + ttest(sample, cutPoint, sampleSize);

    end
      
    disp(builtInTestSuccess/numberOfIterations*100);
    successPercent = successPercent/numberOfIterations*100;
end

function sample = generateSample(sampleSize)
    sample = normrnd(0, 1, [sampleSize/2, 1]);
    sample2 = normrnd(0.3, 1, [sampleSize/2, 1]);
    sample = cat(1, sample, sample2);
end

function success = ttest(sample, cutPoint, sampleSize)
    x = sample(1:cutPoint);
    y = sample(cutPoint+1:sampleSize);
    success = 1-ttest2(x, y);   
end

function success = mwtest(sample, cutPoint, sampleSize)
    x = sample(1:cutPoint);
    y = sample(cutPoint+1:sampleSize);
    [p, h] = ranksum(x, y);
    success = 1-h;
end

function statistic = evaluateStatistic(sample, cutPoint)
    
    a = 0;
    b = 0;
    
    lenght = size(sample);
    lenght = lenght(1);
    
    for t = 1:cutPoint
        a = a + sample(t);
    end
    
    a = a / cutPoint;
    
    for t = cutPoint+1:lenght
        b = b + sample(t);
    end

    b = b / (lenght - cutPoint);
    
    statistic = a - b;
    if (statistic < 0)
        statistic = statistic * -1;
    end
    
end

function statistic = evaluateStatisticMW(sample, cutPoint)
    lenght = size(sample);
    lenght = lenght(1);
    x = sample(1:cutPoint);
    y = sample(cutPoint+1:lenght);
    [p, h, stat] = ranksum(x, y);
    statistic = stat.zval;
    %disp(statistic);
    
    if (statistic < 0)
        statistic = statistic * -1;
    end
end

function h0Accepted = runPermutationTest(sample, alpha, cutPoint, numberOfP)

    statistics = zeros(1, numberOfP);    
    sampleStatistic = evaluateStatisticMW(sample, cutPoint);
    equalOccurences = 0;
    higherOccurences = 0;
    lenght = size(sample);
    lenght = lenght(1);
    
    %disp(alpha);
    %disp(cutPoint);
    %disp(numberOfP);
    %disp(lenght);
    %disp('-');
    
    
    for t = 1:numberOfP
        statistics(t) = evaluateStatisticMW(sample(randperm(lenght)), cutPoint); 
        
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