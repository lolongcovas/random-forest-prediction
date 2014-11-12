function convertPiotrForest2Cpp(model, output_filename)

fileID = fopen(output_filename, 'w');

fprintf(fileID, '#trees: %d\n', numel(model));
fprintf(fileID, 'nLabels %d\n', length(unique(model(1).hs)));

for i=1:numel(model)
    m = model(i);

    fprintf(fileID, 'tree %d\n', i);

    fids = m.fids;
    fprintf(fileID, '%d %d ', length(fids), fids);
    fprintf(fileID, '\n');

    thrs = m.thrs;
    fprintf(fileID, '%d %.9f ', length(thrs), thrs);
    fprintf(fileID, '\n');

    child = m.child;
    fprintf(fileID, '%d %d ', length(child), child);
    fprintf(fileID, '\n');
    
    distr = m.distr;
    fprintf(fileID, '%d %d %.9f ', size(distr,1), size(distr,2), distr);
    fprintf(fileID, '\n');

    hs = m.hs;
    fprintf(fileID, '%d %d ', length(hs), hs);
    fprintf(fileID, '\n');

    count = m.count;
    fprintf(fileID, '%d %d ', length(count), count);
    fprintf(fileID, '\n');

    depth = m.depth;
    fprintf(fileID, '%d %d ', length(depth), depth);
    fprintf(fileID, '\n');
end

fclose(fileID);


